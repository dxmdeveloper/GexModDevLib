#include "gmdlib/graphics/gex/BmpSegmentHeader.hpp"
#include <gmdlib/helpers/binary.hpp>
#include <stdexcept>

namespace gmdlib::gfx::gex
{

    bool BmpSegmentHeader::is_null() const
    {
        return width == 0 || height == 0;
    }

    BmpSegmentHeader::BmpSegmentHeader(std::span<const uint8_t> bin)
    {
        bin_constructor_body(bin.data(), raw_size);
    }

    BmpSegmentHeader::BmpSegmentHeader(
            uint16_t start_offset, uint8_t width, uint8_t height, int16_t rel_pos_x, int16_t rel_pos_y)
            : start_offset{start_offset}, width{width}, height{height}, rel_position_x{rel_pos_x},
              rel_position_y{rel_pos_y} {}

    void BmpSegmentHeader::bin_constructor_body(const void *bin, std::size_t bin_size)
    {
        auto b = reinterpret_cast<const std::byte *>(bin);
        if (bin_size < raw_size)
            throw std::runtime_error("bin_size < bmp_segment.raw_size");

        start_offset = gmdlib::bin::le::read_u16(bin);
        width = gmdlib::bin::le::read_u8(b + 2);
        height = gmdlib::bin::le::read_u8(b + 3);
        rel_position_x = gmdlib::bin::le::read_i16(b + 4);
        rel_position_y = gmdlib::bin::le::read_i16(b + 6);
    }

    BmpSegmentHeader::operator bool() const
    {
        return this->is_null();
    }

    std::istream &operator>>(std::istream &is, BmpSegmentHeader &seg)
    {
        std::array<uint8_t, BmpSegmentHeader::raw_size> buf{};
        is.read(reinterpret_cast<char *>(buf.data()), buf.size());
        seg.bin_constructor_body(buf.data(), buf.size());
        return is;
    }

}