#include "gmdlib/scanning/LEVScanner.hpp"

namespace gmdlib::scanning
{

    uint32_t LEVScanner::gexptr_to_offset(gexptr ptr, uint chunk_ind) const
    {
        auto &ch = chunks.at(chunk_ind);
        if (!ch.exist())
            throw std::runtime_error("No such chunk");
        return ch.gexptr_to_offset(ptr);
    }

    void LEVScanner::set_active_chunk(uint chunk_ind)
    {
        auto &ch = chunks.at(chunk_ind);
        if (!ch.exist())
            throw std::runtime_error("No such chunk");

        active_chunk = ch;
    }

    void LEVScanner::set_active_chunk(enum ChunkType chunk_type)
    {
        set_active_chunk(get_chunk_index(chunk_type));
    }

    uint32_t LEVScanner::gexptr_to_offset(gexptr ptr) const
    {
        return active_chunk.get().gexptr_to_offset(ptr);
    }

    void LEVScanner::read_and_follow_gexptr()
    {
        gexptr p = read_u32();
        follow_gexptr(p);
    }

    uint LEVScanner::get_chunk_index(enum ChunkType type) const
    {
        if (chunks.empty())
            throw std::runtime_error("No chunks");

        if (chunks.size() == 1 && type == ChunkType::main)
            return 0;

        if (chunks.size() == 5 || chunks.size() == 6)
            switch (type) {
                case ChunkType::unknown:
                    return 0;
                case ChunkType::tile_bitmaps:
                    return 1;
                case ChunkType::object_bitmaps:
                    return 2;
                case ChunkType::main:
                    return 3;
                case ChunkType::intro:
                    return 4;
                case ChunkType::background:
                    return 5;
                default:
                    throw std::runtime_error("Invalid chunk type");
            }

        throw std::runtime_error("Unknown file type");
    }

    std::vector<std::vector<uint32_t>> LEVScanner::find_tile_ext_bmps()
    {
        set_active_chunk_and_go_to_ep(ChunkType::tile_bitmaps);

        auto blocks = read_null_term_gexptr_arr();
        std::vector<std::vector<uint32_t>> vector(blocks.size());
        for (int i = 0; i < blocks.size(); i++) {
            follow_gexptr(blocks[i]);
            auto bmp_ptrs = read_null_term_gexptr_arr();
            for (auto p : bmp_ptrs) {
                follow_gexptr(p);
                // It is not known yet if there is need to check both width and height
                if (read_u32() != 0) {
                    vector[i].push_back(gexptr_to_offset(p));
                }
            }
        }
    }

    void LEVScanner::go_to_entry_point()
    {
        stream->seekg(active_chunk.get().get_ep());
    }

    void LEVScanner::set_active_chunk_and_go_to_ep(uint chunk_ind)
    {
        set_active_chunk(chunk_ind);
        go_to_entry_point();
    }

    void LEVScanner::set_active_chunk_and_go_to_ep(enum ChunkType chunk_type)
    {
        set_active_chunk_and_go_to_ep(get_chunk_index(chunk_type));
    }

    std::vector<gexptr> LEVScanner::read_null_term_gexptr_arr(size_t limit)
    {
        std::vector<gexptr> vector;
        auto p = read_u32();
        for (size_t i = 0; p != 0 && i < limit; i++) {
            vector.push_back(p);
            p = read_u32();
        }

        return vector;
    }

    void LEVScanner::follow_gexptr(gexptr ptr)
    {
        uint32_t offset = gexptr_to_offset(ptr);
        if (offset == 0)
            throw std::runtime_error("null gexptr");

        // TODO: more checks
        stream->seekg(offset, std::ios::beg);
    }


}