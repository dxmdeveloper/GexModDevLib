#include "gmdlib/scanning/LEVScanner.hpp"

namespace gmdlib::scanning
{

    uint32_t LEVScanner::gexptr_to_offset(gexptr ptr, uint chunk_ind) const
    {
        auto &ch = m_chunks.at(chunk_ind);
        if (!ch.exist())
            throw std::runtime_error("No such chunk");
        return ch.gexptr_to_offset(ptr);
    }

    void LEVScanner::set_active_chunk(uint chunk_ind)
    {
        auto &ch = m_chunks.at(chunk_ind);
        if (!ch.exist())
            throw std::runtime_error("No such chunk");

        m_active_chunk = ch;
    }

    void LEVScanner::set_active_chunk(enum ChunkType chunk_type)
    {
        set_active_chunk(get_chunk_index(chunk_type));
    }

    uint32_t LEVScanner::gexptr_to_offset(gexptr ptr) const
    {
        return m_active_chunk.get().gexptr_to_offset(ptr);
    }

    void LEVScanner::read_and_follow_gexptr()
    {
        gexptr p = read_u32();
        follow_gexptr(p);
    }

    uint LEVScanner::get_chunk_index(enum ChunkType type) const
    {
        if (m_chunks.empty())
            throw std::runtime_error("No m_chunks");

        if (m_chunks.size() == 1 && type == ChunkType::main)
            return 0;

        if (m_chunks.size() == 5 || m_chunks.size() == 6)
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

    void LEVScanner::go_to_entry_point()
    {
        m_stream->seekg(m_active_chunk.get().get_ep());
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
        m_stream->seekg(offset, std::ios::beg);
    }

    std::vector<std::vector<uint32_t>> LEVScanner::find_tile_ext_bmps()
    {
        set_active_chunk_and_go_to_ep(ChunkType::tile_bitmaps);
        auto blocks = read_null_term_gexptr_arr();
        std::vector<std::vector<uint32_t>> vector(blocks.size());

        for (int i = 0; i < blocks.size(); i++) {
            follow_gexptr(blocks[i]);
            auto bmp_ptrs = read_null_term_gexptr_arr();
            for (auto p: bmp_ptrs) {
                follow_gexptr(p);
                // It is not known yet if there is need to check both m_width and m_height
                if (read_u32() != 0) {
                    vector[i].push_back(gexptr_to_offset(p));
                }
            }
        }
    }


    std::vector<uint32_t> LEVScanner::find_obj_ext_bmps()
    {
        std::vector<uint32_t> vector{};
        std::array<gexptr, 6> blocks{};

        set_active_chunk_and_go_to_ep(ChunkType::object_bitmaps);
        for (auto &b: blocks)
            b = read_u32();

        for (auto b: blocks) {
            follow_gexptr(b);
            auto bmp_ptrs = read_null_term_gexptr_arr();

            for (gexptr p: bmp_ptrs) {
                if (read_u32() != 0) {
                    vector.push_back(gexptr_to_offset(p));
                }
            }
        }

        return vector;
    }

    void LEVScanner::constructor_body()
    {
        auto ep_seek = [&](LEVFileChunkMetadata &metadata){
            m_stream->seekg(metadata.m_chunk_offset + metadata.calc_chunk_padding_size(), std::ios::beg);
            read_and_follow_gexptr();
            return m_stream->tellg();
        };

        uint32_t chunk_cnt = read_u32();
        if (chunk_cnt > 6 /* || chunk_cnt == 0 */) {
            // one chunk file
            LEVFileChunkMetadata metadata{};
            m_stream->seekg(0, std::ios::end);
            metadata.m_chunk_size = m_stream->tellg();
            m_chunks.emplace_back(metadata, ep_seek(metadata));

        } else if (chunk_cnt != 0) {
            // standard level file
            std::array<LEVFileChunkMetadata, 6> metadata {};
            m_stream->seekg(12, std::ios::cur);
            for(int i = 0; i < chunk_cnt; i++)
                *m_stream >> metadata[i];

            for(int i = 0; i < chunk_cnt; i++)
                m_chunks.emplace_back(metadata[i], ep_seek(metadata[i]));

        }
    }

}