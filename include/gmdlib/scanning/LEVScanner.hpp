#pragma once

#include <gmdlib/helpers/BinaryStreamReader.hpp>
#include "IMemStream.hpp"
#include "LevFileChunk.hpp"

using gexptr = uint32_t;

namespace gmdlib::scanning
{
    class LEVScanner : protected bin::le::BinaryStreamReader
    {
    public:
        uint32_t gexptr_to_offset(gexptr ptr, uint chunk_ind) const;

        // constructors
        LEVScanner(const std::string& filename);

        LEVScanner(IMemStream &stream);

    private:
        void set_active_chunk(uint chunk_ind);
        uint32_t gexptr_to_offset(gexptr ptr) const;
        void read_and_follow_gexptr();

    private:
        std::vector<LevFileChunk> chunks;
        std::reference_wrapper<LevFileChunk> active_chunk;
    };

    using IDLScanner = LEVScanner;
}