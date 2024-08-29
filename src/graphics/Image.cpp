#include "gmdlib/graphics/Image.hpp"
#include <gmdlib/common/common.hpp>
#include <cassert>
#include <png.h>

namespace gmdlib::graphics
{
    static void write_png_to_vec(png_structp png_ptr, png_const_bytep data, png_uint_32 bytes_n);
    static void throw_libpng_error(png_struct_def *def, const char *msg);

    ColorRGBA Image::get_pixel(uint x, uint y, bool boundary_check) const
    {
        return bitmap[get_index(x, y, boundary_check)];
    }

    void Image::set_pixel(ColorRGBA color, uint x, uint y, bool boundary_check)
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

    Image::Image(std::pair<int, int> size) : size{size}
    {
        if (size.first == 0 || size.second == 0)
            throw std::runtime_error(err::WIDTH_OR_HEIGHT_EQ_0);
        if (size.first > config::MAX_IMG_WIDTH || size.second > config::MAX_IMG_HEIGHT)
            throw std::runtime_error(err::IMG_WIDTH_OR_HEIGHT_LIMIT_EXCEEDED);

        bitmap = std::make_unique<ColorRGBA[]>(sizeof(ColorRGBA) * size.first * size.second);
    }

    Image::Image(int width, int height) : Image(std::pair<int, int>(width, height)) {}

    std::ptrdiff_t Image::get_index(uint x, uint y, bool boundary_chk) const
    {
        auto [w, h] = size;

        if (boundary_chk) {
            if (x >= w || y >= h) throw std::runtime_error(err::XY_OUT_OF_BOUNDS);
        } else {
            assert(x < w && y < h); // Works only in debug builds
        }

        return y * w + x;
    }

    std::vector<uint8_t> Image::to_png() const
    {
        std::vector<uint8_t> png_data;
        auto [width, height] = size;

        // Initialize PNG structures
        png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, throw_libpng_error, nullptr);
        png_infop info_ptr = png_create_info_struct(png_ptr);

        if(png_ptr == nullptr || info_ptr == nullptr){
            throw std::runtime_error("Failed to initialize PNG structures");
        }

        // Setting IHDR
        png_set_IHDR(png_ptr, info_ptr, width, height, 8,
                     PNG_COLOR_TYPE_RGBA,
                     PNG_INTERLACE_NONE,
                     PNG_COMPRESSION_TYPE_DEFAULT,
                     PNG_FILTER_TYPE_DEFAULT);

        // Custom write function
        png_set_write_fn(png_ptr, &png_data, png_rw_ptr(write_png_to_vec), nullptr);

        // Write info to png structure
        png_write_info(png_ptr, info_ptr);

        // Write png data row by row
        for(int h = 0; h < height; h++){
            png_write_row(png_ptr, (png_const_bytep) &bitmap[h * width]);
        }

        // Finalize
        png_write_end(png_ptr, nullptr);
        png_destroy_write_struct(&png_ptr, &info_ptr);

        return png_data;
    }

    void write_png_to_vec(png_structp png_ptr, png_const_bytep data, png_uint_32 bytes_n)
    {
        auto &vec = *(std::vector<png_byte> *) png_get_io_ptr(png_ptr);
        vec.insert(vec.end(), data, data + bytes_n);
    }

    void throw_libpng_error(png_struct_def *def, const char *msg){
        throw std::runtime_error(msg);
    }

}
