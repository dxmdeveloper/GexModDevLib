#include "gmdlib/graphics/gex/PrimaryGraphicHeader.hpp"
#include "gmdlib/helpers/binary.hpp"
#include <stdexcept>

namespace gmdlib::graphics::gex
{

    int PrimaryGraphicHeader::get_bpp() const
    {
        switch (type_signature & 3) {
            case 2:
                return 16;
            case 1:
                return 8;
            case 0:
                return 4;
        }
        throw std::runtime_error("unknown bpp");
    }

    bool PrimaryGraphicHeader::is_packed() const
    {
        return (type_signature & 4);
    }

    PrimaryGraphicHeader::PrimaryGraphicHeader(Span<const uint8_t> bin)
    {
        bin_constructor_body(bin.data(), bin.size());
    }

    PrimaryGraphicHeader::PrimaryGraphicHeader(
            uint16_t struct_pad, uint32_t img_width, uint32_t img_height,
            int32_t shift_x, int16_t shift_y, uint32_t signature)
            : _struct_pad{struct_pad}, inf_img_width{img_width}, inf_img_height{img_height},
              bitmap_shift_x{shift_x}, bitmap_shift_y{shift_y}, type_signature{signature} {}

    void PrimaryGraphicHeader::bin_constructor_body(const void *bin, std::size_t bin_size)
    {
        auto b = reinterpret_cast<const std::byte *>(bin);
        if (bin_size < raw_size)
            throw std::runtime_error("bin_size < header.raw_size");

        _struct_pad = gmdlib::bin::le::read_u16(b);
        inf_img_width = gmdlib::bin::le::read_u32(b + 2);
        inf_img_height = gmdlib::bin::le::read_u32(b + 6);
        bitmap_shift_x = gmdlib::bin::le::read_i32(b + 10);
        bitmap_shift_y = gmdlib::bin::le::read_i16(b + 14);
        type_signature = gmdlib::bin::le::read_u32(b + 16);
    }

    std::istream &operator>>(std::istream &is, PrimaryGraphicHeader &phdr)
    {
        std::array<uint8_t, PrimaryGraphicHeader::raw_size> buf{};
        is.read(reinterpret_cast<char *>(buf.data()), buf.size());
        phdr.bin_constructor_body(buf.data(), buf.size());
        return is;
    }

}
