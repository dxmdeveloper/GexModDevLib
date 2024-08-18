#include "gmdlib/graphics/Image.hpp"
#include <gmdlib/common/common.hpp>
#include <cassert>

namespace gmdlib::gfx
{

    ColorRGBA Image::get_pixel(int x, int y, bool boundary_check) const
    {
        return bitmap[get_index(x, y, boundary_check)];
    }

    void Image::set_pixel(ColorRGBA color, int x, int y, bool boundary_check)
    {
        bitmap[get_index(x, y, boundary_check)] = color;
    }

    ColorRGBA *Image::get_bitmap_ptr() const { return bitmap.get(); }


    std::unique_ptr<ColorRGBA[]> Image::get_bitmap_copy() const
    {
        size_t byte_size = sizeof(ColorRGBA) * size.first * size.second;
        auto new_bmp = std::make_unique<ColorRGBA[]>(byte_size);
        std::copy(bitmap.get(), bitmap.get() + size.first * size.second, new_bmp.get());
        return new_bmp;
    }

    Image::Image(std::pair<int, int> size) : size{size} {
        if(size.first == 0 || size.second == 0)
            throw std::runtime_error(err::WIDTH_OR_HEIGHT_EQ_0);
        if(size.first > config::MAX_IMG_WIDTH || size.second > config::MAX_IMG_HEIGHT)
            throw std::runtime_error(err::IMG_WIDTH_OR_HEIGHT_LIMIT_EXCEEDED);

        bitmap = std::make_unique<ColorRGBA[]>(sizeof(ColorRGBA) * size.first * size.second);
    }

    Image::Image(int width, int height) : Image(std::pair<int, int>(width, height)) {}

    std::ptrdiff_t Image::get_index(int x, int y, bool boundary_chk) const
    {
        auto [w, h] = size;

        if (boundary_chk) {
            if (x >= w || y >= h) throw std::runtime_error(err::XY_OUT_OF_BOUNDS);
        } else {
            assert(x < w && y < h); // Works only in debug builds
        }

        return y * w + x;
    }


}
