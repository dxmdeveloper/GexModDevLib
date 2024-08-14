#pragma once

#include <gmdlib/graphics/Image.hpp>

namespace gmdlib::gfx::gex
{

    // Virtual class
    class Graphic
    {
    public:
        virtual Image draw() = 0;

    };
}