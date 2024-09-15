#pragma once

#include <gmdlib/helpers/BinaryStreamReader.hpp>
#include "MemIStream.hpp"
#include "LevFileChunk.hpp"
#include <limits>

using gexptr = uint32_t;

namespace gmdlib::scanning
{
    enum class ChunkType {
        unknown,
        tile_bitmaps,
        object_bitmaps,
        main,
        intro,
        background
    };

    class LEVScanner : protected bin::le::BinaryStreamReader
    {
    public:
        uint32_t gexptr_to_offset(gexptr ptr, uint chunk_ind) const;

        uint get_chunk_index(enum ChunkType type) const;

        // constructors
        LEVScanner(std::string_view filename);

        LEVScanner(MemIStream &stream);

    private:
        void set_active_chunk(uint chunk_ind);
        void set_active_chunk(enum ChunkType chunk_type);
        void go_to_entry_point();
        void set_active_chunk_and_go_to_ep(uint chunk_ind);
        void set_active_chunk_and_go_to_ep(enum ChunkType chunk_type);
        uint32_t gexptr_to_offset(gexptr ptr) const;
        void read_and_follow_gexptr();
        void follow_gexptr(gexptr ptr);
        std::vector<gexptr> read_null_term_gexptr_arr(size_t limit = INT16_MAX);

        std::vector<std::vector<uint32_t>> find_tile_ext_bmps();
        std::vector<uint32_t> find_obj_ext_bmps();

        void constructor_body();

    private:
        std::vector<LevFileChunk> m_chunks;
        std::reference_wrapper<LevFileChunk> m_active_chunk;
    };

    using IDLScanner = LEVScanner;
}