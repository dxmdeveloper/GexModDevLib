#pragma once

#include <cstddef>

namespace gmdlib::graphics::gex
{
    enum class GraphicCategory
    {
        tile,
        object,
        intro_tile,
        intro_object,
        background
    };
    constexpr size_t GRAPHIC_CATEGORIES_CNT = 5;
}