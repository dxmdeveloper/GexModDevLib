#pragma once

#include <cstdint>
#include <span>
#include <istream>

namespace gmdlib::gfx::gex
{
    struct PrimaryGraphicHeader
    {
        uint16_t _struct_pad = 0;
        uint32_t inf_img_width = 0;
        uint32_t inf_img_height = 0;
        int32_t bitmap_shift_x = 0;
        int16_t bitmap_shift_y = 0;
        uint32_t type_signature = 0;

        static constexpr std::size_t raw_size = 20;

    public:
        int get_bpp() const;
        bool is_packed() const;

        // constructors
        PrimaryGraphicHeader() = default;
        PrimaryGraphicHeader(uint16_t struct_pad, uint32_t img_width, uint32_t img_height,
                             int32_t shift_x, int16_t shift_y, uint32_t signature);
        explicit PrimaryGraphicHeader(std::span<const uint8_t> bin);

        // operator overloads
        friend std::istream &operator>>(std::istream &is, PrimaryGraphicHeader &phdr);

    private:
        void bin_constructor_body(const void *bin, std::size_t bin_size);
    };
}
