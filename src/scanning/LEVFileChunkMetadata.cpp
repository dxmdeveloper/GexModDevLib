#include "gmdlib/scanning/LEVFileChunkMetadata.hpp"
#include <gmdlib/helpers/BinaryStreamReader.hpp>
#include <gmdlib/helpers/binary.hpp>
#include <gmdlib/common/common.hpp>

namespace gmdlib::scanning
{
    std::istream &operator>>(std::istream &is, LEVFileChunkMetadata &metadata)
    {
        bin::le::BinaryStreamReader reader(&is);
        reader >> metadata.file_id;
        reader >> metadata.chunk_id;
        reader >> metadata.chunk_size;
        reader >> metadata.chunk_offset;
        return is;
    }

    LEVFileChunkMetadata::LEVFileChunkMetadata(uint32_t file_id, uint32_t chunk_id, uint32_t chunk_size,
                                               uint32_t chunk_offset)
            : file_id(file_id), chunk_id(chunk_id), chunk_size(chunk_size), chunk_offset(chunk_offset) {}

    LEVFileChunkMetadata::LEVFileChunkMetadata(Span<uint8_t> data)
    {
        static_assert(sizeof(LEVFileChunkMetadata) == 16);
        if (data.size() != sizeof(LEVFileChunkMetadata))
            throw std::runtime_error(err::UNEXPECTED_END_OF_DATA);

        file_id = bin::le::read_u32(data.data());
        chunk_id = bin::le::read_u32(data.data() + 4);
        chunk_size = bin::le::read_u32(data.data() + 8);
        chunk_offset = bin::le::read_u32(data.data() + 12);
    }

    LEVFileChunkMetadata::LEVFileChunkMetadata(std::istream &is)
    {
        is >> *this;
    }


}