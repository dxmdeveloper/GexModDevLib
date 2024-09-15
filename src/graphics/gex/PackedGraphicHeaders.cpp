#include "gmdlib/graphics/gex/PackedGraphicHeaders.hpp"
#include <gmdlib/helpers/BinaryStreamReader.hpp>
#include <numeric>

namespace gmdlib::graphics::gex
{
    size_t PackedGraphicHeaders::calc_bitmap_size() const
    {
        auto bpp = get_bpp();
        auto sum_func = [bpp](const int &a, const uint8_t &b) {
            if (b >= 0x80) return a + bpp / 2;
            return a + (b == 0 ? 4096 / bpp : b * 32 / bpp);
        };
        return std::accumulate(m_unpacking_process_data.begin(), m_unpacking_process_data.end(), 0, sum_func);
    }

    PackedGraphicHeaders::PackedGraphicHeaders(Span<const uint8_t> bin)
            : BasicGraphicHeaders(bin)
    {
        if(!m_prim_hdr.is_packed())
            throw std::runtime_error(err::INVALID_FORMAT_GRAPHIC_TYPE);

        bin = bin.subspan(BasicGraphicHeaders::get_raw_size_of_headers());
        if (bin.size() < 2)
            throw std::runtime_error(err::UNEXPECTED_END_OF_DATA);

        uint32_t upkg_dat_len = gmdlib::bin::le::read_u32(bin.data());

        if (upkg_dat_len > config::MAX_UNPKG_DATA_LEN)
            throw std::runtime_error(err::UNPKG_DATA_LEN_LIMIT_EXCEEDED);

        if (bin.size() < upkg_dat_len)
            throw std::runtime_error(err::UNEXPECTED_END_OF_DATA);

        bin = bin.subspan(sizeof(upkg_dat_len), upkg_dat_len - sizeof(upkg_dat_len));
        m_unpacking_process_data = std::vector<uint8_t>(upkg_dat_len - sizeof(upkg_dat_len));
        std::copy(bin.begin(), bin.end(), m_unpacking_process_data.data());
    }

    PackedGraphicHeaders::PackedGraphicHeaders(std::istream &is) : BasicGraphicHeaders(is)
    {
        if(!m_prim_hdr.is_packed())
            throw std::runtime_error(err::INVALID_FORMAT_GRAPHIC_TYPE);

        bin::le::BinaryStreamReader reader(&is);
        uint32_t upkg_dat_len;
        reader >> upkg_dat_len;
        upkg_dat_len -= sizeof(upkg_dat_len); // 4 bytes

        if (upkg_dat_len > config::MAX_UNPKG_DATA_LEN)
            throw std::runtime_error(err::UNPKG_DATA_LEN_LIMIT_EXCEEDED);

        m_unpacking_process_data = std::vector<uint8_t>(upkg_dat_len);
        reader.read(m_unpacking_process_data, upkg_dat_len);
    }

    std::istream &operator>>(std::istream &is, PackedGraphicHeaders &phdrs)
    {
        phdrs = PackedGraphicHeaders(is);
        return is;
    }

}