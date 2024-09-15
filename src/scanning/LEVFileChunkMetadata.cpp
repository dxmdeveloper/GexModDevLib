#include "gmdlib/scanning/LEVFileChunkMetadata.hpp"
#include <gmdlib/helpers/BinaryStreamReader.hpp>
#include <gmdlib/helpers/binary.hpp>
#include <gmdlib/common/common.hpp>

namespace gmdlib::scanning
{
    uint32_t LEVFileChunkMetadata::calc_chunk_padding_size()
    {
        return m_chunk_size / 2048 + 4;
    }

    std::istream &operator>>(std::istream &is, LEVFileChunkMetadata &metadata)
    {
        bin::le::BinaryStreamReader reader(&is);
        reader >> metadata.m_file_id;
        reader >> metadata.m_chunk_id;
        reader >> metadata.m_chunk_size;
        reader >> metadata.m_chunk_offset;
        return is;
    }

    LEVFileChunkMetadata::LEVFileChunkMetadata(uint32_t file_id, uint32_t chunk_id, uint32_t chunk_size,
                                               uint32_t chunk_offset)
            : m_file_id(file_id), m_chunk_id(chunk_id), m_chunk_size(chunk_size), m_chunk_offset(chunk_offset) {}

    LEVFileChunkMetadata::LEVFileChunkMetadata(Span<uint8_t> data)
    {
        static_assert(sizeof(LEVFileChunkMetadata) == 16);
        if (data.size() != sizeof(LEVFileChunkMetadata))
            throw std::runtime_error(err::UNEXPECTED_END_OF_DATA);

        m_file_id = bin::le::read_u32(data.data());
        m_chunk_id = bin::le::read_u32(data.data() + 4);
        m_chunk_size = bin::le::read_u32(data.data() + 8);
        m_chunk_offset = bin::le::read_u32(data.data() + 12);
    }

    LEVFileChunkMetadata::LEVFileChunkMetadata(std::istream &is)
    {
        is >> *this;
    }

}