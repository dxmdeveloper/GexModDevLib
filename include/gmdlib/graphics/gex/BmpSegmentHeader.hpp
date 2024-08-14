#pragma once

#include <cstdint>
#include <span>
#include <istream>

namespace gmdlib::gfx::gex
{

    struct BmpSegmentHeader
    {
        uint16_t start_offset = 0;
        uint8_t width = 0;
        uint8_t height = 0;
        int16_t rel_position_x = 0;
        int16_t rel_position_y = 0;

        static constexpr std::size_t raw_size = 8;

    public:
        bool is_null() const;

        // constructors
        BmpSegmentHeader() = default;
        BmpSegmentHeader(uint16_t start_offset, uint8_t width, uint8_t height, int16_t rel_pos_x, int16_t rel_pos_y);
        explicit BmpSegmentHeader(std::span<const uint8_t> bin);

        // operator overloads
        explicit operator bool() const;
        friend std::istream &operator>>(std::istream &is, BmpSegmentHeader &seg);

    private:
        void bin_constructor_body(const void *bin, std::size_t bin_size);
    };
}
