#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include "GraphicCategory.hpp"

namespace gmdlib::graphics::gex
{
    struct GraphicInfo
    {
        uint32_t m_graphic_offset;
        uint32_t m_palette_offset;
        std::vector<uint32_t> m_separated_bitmap_offsets;
        GraphicCategory m_category;

    public:
        virtual std::string to_string() const = 0;
    };
}
