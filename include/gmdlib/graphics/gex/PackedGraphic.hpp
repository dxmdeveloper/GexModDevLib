#pragma once

#include "Graphic.hpp"
#include "PackedGraphicHeaders.hpp"

namespace gmdlib::gfx::gex
{

    struct PackedGraphic : gex::Graphic
    {
        PackedGraphicHeaders headers;
        std::vector<uint8_t> bitmap;
    };

}