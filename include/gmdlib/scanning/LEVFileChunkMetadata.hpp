#include <cstdint>
#include <istream>
#include <gmdlib/common/Span.hpp>

namespace gmdlib::scanning
{
    struct LEVFileChunkMetadata
    {
        uint32_t file_id;
        uint32_t chunk_id;
        uint32_t chunk_size;
        uint32_t chunk_offset;

    public:
        LEVFileChunkMetadata() = default;
        LEVFileChunkMetadata(uint32_t file_id, uint32_t chunk_id, uint32_t chunk_size, uint32_t chunk_offset);
        LEVFileChunkMetadata(Span<uint8_t> data);
        LEVFileChunkMetadata(std::istream& is);

        friend std::istream& operator>>(std::istream& is, LEVFileChunkMetadata& metadata);
    };
}