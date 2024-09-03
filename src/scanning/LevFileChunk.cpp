#include "gmdlib/scanning/LevFileChunk.hpp"

namespace gmdlib::scanning
{
    uint32_t LevFileChunk::get_ep() const { return entry_point_offset; }

    void LevFileChunk::set_ep(uint32_t ep) { entry_point_offset = ep; }

    uint32_t LevFileChunk::get_size() const { return chunk_size; }

    uint32_t LevFileChunk::get_offset() const { return chunk_offset; }
}