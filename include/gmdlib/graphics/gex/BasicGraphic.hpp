#pragma once

#include "Graphic.hpp"
#include "BasicGraphicHeaders.hpp"

namespace gmdlib::gfx::gex
{

    struct BasicGraphic : gex::Graphic
    {
        BasicGraphicHeaders headers;
        std::vector<uint8_t> bitmap;
    };

}