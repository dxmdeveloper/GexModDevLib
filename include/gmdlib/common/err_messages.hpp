#pragma once

namespace gmdlib::err
{
    constexpr char UNEXPECTED_END_OF_DATA[]{"Unexpected end of data/file."};
    constexpr char UNPKG_DATA_LEN_LIMIT_EXCEEDED[]{
            "Unpacking process data length exceeded. Graphic might be corrupted."};
    constexpr char BITMAP_BIN_SIZE_LIMIT_EXCEEDED[]{"Bitmap binary size limit exceeded"};
    constexpr char INDEX_OUT_OF_BOUNDS[]{"Index out of array bounds."};
    constexpr char XY_OUT_OF_BOUNDS[]{"x or y out of bounds."};
    constexpr char UNEXPECTED_VALUE[]{"unexpected value"};
    constexpr char NOT_IMPLEMENTED[]{"not implemented"};
    constexpr char UNEXPECTED_NEGATIVE_RESULT[]{"unexpected negative result"};
    constexpr char UNINITIALIZED_STRUCT[]{"structure is uninitialized"};
    constexpr char WIDTH_OR_HEIGHT_EQ_0[]{"width/height equals 0"};
    constexpr char IMG_WIDTH_OR_HEIGHT_LIMIT_EXCEEDED[]{"image width/height limit exceeded"};
    constexpr char COLOR_PALETTE_NOT_ASSIGNED[]{"Color palette is not assigned"};
    constexpr char COLOR_PALETTE_INCOMPATIBLE[]{
            "Color palette is incompatible (different count of colors than expected)"};
    constexpr char INVALID_FORMAT_GRAPHIC_TYPE[]{"Graphic is of a different type than expected"};
}