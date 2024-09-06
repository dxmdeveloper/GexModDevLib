#include "gmdlib/scanning/LevFileChunk.hpp"

namespace gmdlib::scanning
{
    bool LevFileChunk::exist() const
    {
        return get_size() > sizeof("dummy");
    }

    uint32_t LevFileChunk::get_ep() const { return entry_point_offset; }

    void LevFileChunk::set_ep(uint32_t entry_point) { entry_point_offset = entry_point; }

    uint32_t LevFileChunk::get_size() const { return chunk_size; }

    uint32_t LevFileChunk::get_offset() const { return chunk_offset; }

    uint32_t LevFileChunk::gexptr_to_offset(gexptr ptr) const
    {
        if(!exist())
            throw std::out_of_range("gexptr_to_offset: chunk does not exist");

        if (ptr == 0) return 0;
        return get_offset() + (ptr >> 20) * 0x2000 + (ptr & 0xFFFF) - 1;
    }

    gexptr LevFileChunk::offset_to_gexptr(uint32_t offset) const
    {
        if(!exist())
            throw std::out_of_range("gexptr_to_offset: chunk does not exist");

        offset -= get_offset();
        return ((offset >> 13) << 20) + (offset & 0x1FFF) + 1;
    }

    LevFileChunk::operator bool() const
    {
        return exist();
    }
}// namespace gmdlib::scanning
