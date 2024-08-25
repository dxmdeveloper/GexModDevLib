#pragma once

#include "Graphic.hpp"
#include "BasicGraphicHeaders.hpp"

namespace gmdlib::gfx::gex
{

    struct BasicGraphic : public gex::Graphic
    {
        //TODO: PUT IT IN CONSTRUCTORS
        // if(prim_hdr.is_packed())
        //            throw std::runtime_error(err::INVALID_FORMAT_GRAPHIC_TYPE);

        BasicGraphicHeaders headers;
        std::vector<uint8_t> bitmap;
    };

}