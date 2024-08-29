#pragma once

#include <vector>
#include "BmpSegmentHeader.hpp"
#include "PrimaryGraphicHeader.hpp"

namespace gmdlib::graphics::gex
{

    struct BasicGraphicHeaders
    {
        PrimaryGraphicHeader prim_hdr;
        std::vector<BmpSegmentHeader> bmp_seg_hdrs; // null terminated

    public:
        std::pair<int, int> calc_dimensions() const;
        virtual size_t calc_bitmap_size() const;

        size_t get_raw_size_of_headers() const;
        int get_bpp() const;


        BasicGraphicHeaders() = default;
        explicit BasicGraphicHeaders(std::istream &is);
        BasicGraphicHeaders(PrimaryGraphicHeader prim_hdr, Span<const BmpSegmentHeader> bmp_seg_hdrs);

        /// @param bin binary data consisting PrimaryGraphicHeader and null terminated array of BmpSegmentHeader objects.
        explicit BasicGraphicHeaders(Span<const uint8_t> bin);

        friend std::istream &operator>>(std::istream &is, BasicGraphicHeaders &ghdr);

    };
}