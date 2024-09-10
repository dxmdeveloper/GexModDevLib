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

    uint32_t LEVScanner::gexptr_to_offset(gexptr ptr) const
    {
        return active_chunk.get().gexptr_to_offset(ptr);
    }

    void LEVScanner::read_and_follow_gexptr()
    {
        gexptr p = read_u32();
        uint32_t offset = gexptr_to_offset(p);
        if (offset == 0)
            throw std::runtime_error("null gexptr");

        stream->seekg(offset, std::ios::beg);
    }
}