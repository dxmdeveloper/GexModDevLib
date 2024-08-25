// header only
#pragma once

#include <cstdint>

using BGR555 = uint16_t;

namespace gmdlib::gfx
{
    struct ColorRGBA
    {
        uint8_t red = 0;
        uint8_t green = 0;
        uint8_t blue = 0;
        uint8_t alpha = 255;

    public:
        bool is_fully_transparent() const { return alpha == 0; }

        ColorRGBA() = default;
        explicit ColorRGBA(BGR555 bgr555)
        {
            if (bgr555 == 0) {
                alpha = 0;
                return;
            }
            red = 8 * ((bgr555) & 0b11111);
            green = 8 * ((bgr555 >> 5) & 0b11111);
            blue = 8 * ((bgr555 >> 10) & 0b11111);
        }

        static inline ColorRGBA from_RGB555(BGR555 value)
        {
            return ColorRGBA(value);
        }
    };
    static_assert(sizeof(ColorRGBA) == 4);
}