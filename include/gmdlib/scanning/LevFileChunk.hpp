#pragma once
#include "LEVFileChunkMetadata.hpp"

using gexptr = uint32_t;

namespace gmdlib::scanning
{

    class LevFileChunk : private LEVFileChunkMetadata
    {
    public:
        [[nodiscard]] bool exist() const;
        uint32_t get_ep() const;
        void set_ep(uint32_t entry_point);
        uint32_t get_size() const;
        uint32_t get_offset() const;
        uint32_t gexptr_to_offset(gexptr ptr) const;
        [[maybe_unused]] gexptr offset_to_gexptr(uint32_t offset) const;

        LevFileChunk(LEVFileChunkMetadata metadata, uint32_t entry_point);;
        explicit operator bool() const;
    private:
        uint32_t m_entry_point_offset;
    };

}
