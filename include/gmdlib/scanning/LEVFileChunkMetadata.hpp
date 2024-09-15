#include <cstdint>
#include <istream>
#include <gmdlib/common/Span.hpp>

namespace gmdlib::scanning
{
    struct LEVFileChunkMetadata
    {
        uint32_t m_file_id;
        uint32_t m_chunk_id;
        uint32_t m_chunk_size;
        uint32_t m_chunk_offset;

    public:
        uint32_t calc_chunk_padding_size();

        LEVFileChunkMetadata() = default;
        LEVFileChunkMetadata(uint32_t file_id, uint32_t chunk_id, uint32_t chunk_size, uint32_t chunk_offset);
        explicit LEVFileChunkMetadata(Span<uint8_t> data);
        explicit LEVFileChunkMetadata(std::istream& is);

        friend std::istream& operator>>(std::istream& is, LEVFileChunkMetadata& metadata);
    };
}