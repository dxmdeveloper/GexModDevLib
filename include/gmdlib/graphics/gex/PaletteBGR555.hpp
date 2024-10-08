#pragma once

#include <vector>
#include <cstdint>
#include <gmdlib/common/Span.hpp>
#include <istream>

using BGR555 = uint16_t;

namespace gmdlib::graphics::gex
{

    // m_palette binary structure:
    // * 1 byte - count of m_colors. 0 => 16 m_colors, 1 => 256 m_colors, 2 => 0 m_colors (assigned to a 16bpp graphic)
    // * 3 bytes - padding (usually 0xFFFFFF or 0xFFFF00 in some unfinished levels)
    // * array of m_colors in BGR555 format (2 bytes for each color)
    struct PaletteBGR555
    {
        std::vector<BGR555> m_colors;

    public:
        [[nodiscard]] size_t size() const;
        [[nodiscard]] BGR555 at(uint ind) const;

    public:
        PaletteBGR555() = default;
        explicit PaletteBGR555(Span<uint8_t> bin);
        explicit PaletteBGR555(std::istream &is);

        BGR555 operator[](uint ind);
        friend std::istream &operator>>(std::istream &is, PaletteBGR555 &pal);
    };
}