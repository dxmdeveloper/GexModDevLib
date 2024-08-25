#include <gmdlib/graphics/gex/BasicGraphicHeaders.hpp>
#include <gmdlib/common/common.hpp>
#include <stdexcept>
#include <algorithm>

namespace gmdlib::gfx::gex
{

    size_t BasicGraphicHeaders::get_raw_size_of_headers() const
    {
        return PrimaryGraphicHeader::raw_size + BmpSegmentHeader::raw_size * bmp_seg_hdrs.size();
    }

    int BasicGraphicHeaders::get_bpp() const
    {
        return prim_hdr.get_bpp();
    }

    BasicGraphicHeaders::BasicGraphicHeaders(PrimaryGraphicHeader prim_hdr,
                                             std::span<const BmpSegmentHeader> bmp_seg_hdrs)
            : prim_hdr{prim_hdr}, bmp_seg_hdrs(bmp_seg_hdrs.begin(), bmp_seg_hdrs.end()) {}

    BasicGraphicHeaders::BasicGraphicHeaders(std::span<const uint8_t> bin)
    {
        constexpr size_t phdr_size{PrimaryGraphicHeader::raw_size};
        constexpr size_t seg_size{BmpSegmentHeader::raw_size};
        BmpSegmentHeader seg(bin.subspan<phdr_size>());

        if (bin.size() < phdr_size + seg_size)
            throw std::runtime_error("binary array is too small to create any BasicGraphicHeaders object");

        prim_hdr = PrimaryGraphicHeader(bin);

        int seg_cnt = 1;
        for (; !seg.is_null(); seg_cnt++) {
            if (bin.size() < phdr_size + seg_size * (seg_cnt + 1))
                throw std::runtime_error("binary array is too small to create another BmpSegmentHeader object");
            if (seg_cnt + 1 > config::MAX_BMP_SEGMENTS)
                throw std::runtime_error("MAX_BMP_SEGMENTS limit exceeded");

            seg = BmpSegmentHeader({bin.begin() + phdr_size + seg_size * seg_cnt, bin.end()});
        }
        bmp_seg_hdrs = std::vector<BmpSegmentHeader>(seg_cnt);
        for (int i = 0; i < seg_cnt; i++) {
            bmp_seg_hdrs[i] = BmpSegmentHeader({bin.begin() + phdr_size + seg_size * i, bin.end()});
        }
    }

    std::istream &operator>>(std::istream &is, BasicGraphicHeaders &ghdr)
    {
        BmpSegmentHeader seg;
        is >> ghdr.prim_hdr;
        is >> seg;

        for (int seg_cnt = 1; !seg.is_null(); seg_cnt++) {
            ghdr.bmp_seg_hdrs.push_back(seg);
            if (seg_cnt + 1 > config::MAX_BMP_SEGMENTS)
                throw std::runtime_error("MAX_BMP_SEGMENTS limit exceeded");

            is >> seg;
        }
        ghdr.bmp_seg_hdrs.shrink_to_fit();
        return is;
    }

    BasicGraphicHeaders::BasicGraphicHeaders(std::istream &is)
    {
        is >> *this;
    }

    std::pair<int, int> BasicGraphicHeaders::calc_dimensions() const
    {
        if (bmp_seg_hdrs.empty())
            throw std::runtime_error(err::UNINITIALIZED_STRUCT);

        auto wmm = std::max_element(bmp_seg_hdrs.begin(), bmp_seg_hdrs.end() - 1,
                                    [](auto &a, auto &b) {
                                        return a.width + a.rel_position_x < b.width + b.rel_position_x;
                                    });

        auto hmm = std::max_element(bmp_seg_hdrs.begin(), bmp_seg_hdrs.end() - 1,
                                    [](auto &a, auto &b) {
                                        return a.height + a.rel_position_y < b.height + b.rel_position_y;
                                    });

        int w = wmm->rel_position_x + wmm->width;
        int h = hmm->rel_position_y + hmm->height;

        if (w < 0 || h < 0)
            throw std::runtime_error(err::UNEXPECTED_NEGATIVE_RESULT);

        return {w, h};
    }

}