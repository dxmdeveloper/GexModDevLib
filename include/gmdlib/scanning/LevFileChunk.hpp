#pragma once
#include "LEVFileChunkMetadata.hpp"

namespace gmdlib::scanning
{

    class LevFileChunk : private LEVFileChunkMetadata
    {
    public:
        uint32_t get_ep() const;
        void set_ep(uint32_t ep);
        uint32_t get_size() const;
        uint32_t get_offset() const;
    private:
        uint32_t entry_point_offset;
    };

}