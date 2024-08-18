#pragma once

#include <memory>
#include "Color.hpp"

namespace gmdlib::gfx
{
    class Image
    {
    public:
        ColorRGBA get_pixel(int x, int y, bool boundary_check = true) const;
        void set_pixel(ColorRGBA color, int x, int y, bool boundary_check = true);
        ColorRGBA *get_bitmap_ptr() const;
        std::unique_ptr<ColorRGBA[]> get_bitmap_copy() const;

        // Constructors
        explicit Image(std::pair<int, int> size);
        Image(int width, int height);

    private:
        std::ptrdiff_t get_index(int x, int y, bool boundary_chk) const;

    private:
        std::pair<int, int> size;
        std::unique_ptr<ColorRGBA[]> bitmap;
    };
}
