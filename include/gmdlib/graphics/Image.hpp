#pragma once

#include <memory>
#include <vector>
#include "Color.hpp"

using uint = unsigned int;

namespace gmdlib::graphics
{
    class Image
    {
    public:
        ColorRGBA get_pixel(uint x, uint y, bool boundary_check = true) const;
        void set_pixel(ColorRGBA color, uint x, uint y, bool boundary_check = true);
        ColorRGBA *get_bitmap_ptr() const;
        std::unique_ptr<ColorRGBA[]> get_bitmap_copy() const;
        std::vector<uint8_t> to_png() const;

        // Constructors
        explicit Image(std::pair<int, int> size);
        Image(int width, int height);

    private:
        std::ptrdiff_t get_index(uint x, uint y, bool boundary_chk) const;

    private:
        std::pair<int, int> m_size;
        std::unique_ptr<ColorRGBA[]> m_bitmap;
    };
}
