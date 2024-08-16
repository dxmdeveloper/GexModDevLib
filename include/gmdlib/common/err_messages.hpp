#pragma once

namespace gmdlib::err
{
    constexpr char UNEXPECTED_END_OF_DATA[]{"Unexpected end of data/file."};
    constexpr char UNPKG_DATA_LEN_LIMIT_EXCEEDED[]{
            "Unpacking process data length exceeded. Graphic might be corrupted."};
    constexpr char INDEX_OUT_OF_BOUNDS[]{"Index out of array bounds."};
    constexpr char XY_OUT_OF_BOUNDS[]{"x or y out of bounds."};
    constexpr char UNEXPECTED_VALUE[]{"unexpected value"};
}