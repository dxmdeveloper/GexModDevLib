#include "gmdlib/graphics/gex/PackedGraphicHeaders.hpp"
#include <gmdlib/helpers/BinaryStreamReader.hpp>

namespace gmdlib::gfx::gex
{
    PackedGraphicHeaders::PackedGraphicHeaders(std::span<const uint8_t> bin)
            : BasicGraphicHeaders(bin)
    {
        bin = bin.subspan(BasicGraphicHeaders::get_raw_size_of_headers());
        if (bin.size() < 2)
            throw std::runtime_error(err::UNEXPECTED_END_OF_DATA);

        uint32_t upkg_dat_len = gmdlib::bin::le::read_u32(bin.data());

        if (upkg_dat_len > config::MAX_UNPKG_DATA_LEN)
            throw std::runtime_error(err::UNPKG_DATA_LEN_LIMIT_EXCEEDED);

        if (bin.size() < upkg_dat_len)
            throw std::runtime_error(err::UNEXPECTED_END_OF_DATA);

        bin = bin.subspan(sizeof(upkg_dat_len), upkg_dat_len - sizeof(upkg_dat_len));
        unpacking_process_data = std::vector<uint8_t>(upkg_dat_len - sizeof(upkg_dat_len));
        std::copy(bin.begin(), bin.end(), unpacking_process_data.data());
    }

    PackedGraphicHeaders::PackedGraphicHeaders(std::istream &is) : BasicGraphicHeaders(is)
    {
        bin::le::BinaryStreamReader reader(&is);
        uint32_t upkg_dat_len;
        reader >> upkg_dat_len;

        if (upkg_dat_len > config::MAX_UNPKG_DATA_LEN)
            throw std::runtime_error(err::UNPKG_DATA_LEN_LIMIT_EXCEEDED);

        unpacking_process_data = std::vector<uint8_t>(upkg_dat_len);
        reader.read(unpacking_process_data, upkg_dat_len);
    }

    std::istream &operator>>(std::istream &is, PackedGraphicHeaders &phdrs)
    {
        phdrs = PackedGraphicHeaders(is);
        return is;
    }

}