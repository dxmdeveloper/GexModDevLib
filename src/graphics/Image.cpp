#include "gmdlib/graphics/Image.hpp"
#include <gmdlib/common/common.hpp>

namespace gmdlib::gfx
{

    ColorRGBA Image::get_pixel(int x, int y) const
    {
        return bitmap[get_index(x, y)];
    }

    void Image::set_pixel(ColorRGBA color, int x, int y)
    {
        bitmap[get_index(x, y)] = color;
    }

    ColorRGBA *Image::get_bitmap_ptr() const { return bitmap.get(); }


    std::unique_ptr<ColorRGBA[]> Image::get_bitmap_copy() const
    {
        size_t byte_size = sizeof(ColorRGBA) * size.first * size.second;
        auto new_bmp = std::make_unique<ColorRGBA[]>(byte_size);
        std::copy(bitmap.get(), bitmap.get() + size.first * size.second, new_bmp.get());
        return new_bmp;
    }

    Image::Image(std::pair<int, int> size) : size{size}, bitmap{std::make_unique<ColorRGBA[]>(
            sizeof(ColorRGBA) * size.first * size.second)} {}

    Image::Image(int width, int height) : Image(std::pair<int, int>(width, height)) {}

    std::ptrdiff_t Image::get_index(int x, int y) const
    {
        auto [w, h] = size;
        if (x >= w || y >= h)
            throw std::runtime_error(err::XY_OUT_OF_BOUNDS);

        return y * w + x;
    }


}
