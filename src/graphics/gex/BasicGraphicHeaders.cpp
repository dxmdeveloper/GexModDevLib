#include <gmdlib/graphics/gex/BasicGraphicHeaders.hpp>
#include <gmdlib/common/common.hpp>
#include <stdexcept>
#include <algorithm>
#include <numeric>

namespace gmdlib::graphics::gex
{

    size_t BasicGraphicHeaders::get_raw_size_of_headers() const
    {
        return PrimaryGraphicHeader::raw_size + BmpSegmentHeader::raw_size * m_bmp_seg_hdrs.size();
    }

    int BasicGraphicHeaders::get_bpp() const
    {
        return m_prim_hdr.get_bpp();
    }

    BasicGraphicHeaders::BasicGraphicHeaders(PrimaryGraphicHeader prim_hdr,
                                             Span<const BmpSegmentHeader> bmp_seg_hdrs)
            : m_prim_hdr{prim_hdr}, m_bmp_seg_hdrs(bmp_seg_hdrs.begin(), bmp_seg_hdrs.end()) {}

    BasicGraphicHeaders::BasicGraphicHeaders(Span<const uint8_t> bin)
    {
        constexpr size_t phdr_size{PrimaryGraphicHeader::raw_size};
        constexpr size_t seg_size{BmpSegmentHeader::raw_size};
        BmpSegmentHeader seg(bin.subspan<phdr_size>());

        if (bin.size() < phdr_size + seg_size)
            throw std::runtime_error("binary array is too small to create any BasicGraphicHeaders object");

        m_prim_hdr = PrimaryGraphicHeader(bin);

        int seg_cnt = 1;
        for (; !seg.is_null(); seg_cnt++) {
            if (bin.size() < phdr_size + seg_size * (seg_cnt + 1))
                throw std::runtime_error("binary array is too small to create another BmpSegmentHeader object");
            if (seg_cnt + 1 > config::MAX_BMP_SEGMENTS)
                throw std::runtime_error("MAX_BMP_SEGMENTS limit exceeded");

            seg = BmpSegmentHeader({bin.begin() + phdr_size + seg_size * seg_cnt, bin.end()});
        }
        m_bmp_seg_hdrs = std::vector<BmpSegmentHeader>(seg_cnt);
        for (int i = 0; i < seg_cnt; i++) {
            m_bmp_seg_hdrs[i] = BmpSegmentHeader({bin.begin() + phdr_size + seg_size * i, bin.end()});
        }
    }

    std::istream &operator>>(std::istream &is, BasicGraphicHeaders &ghdr)
    {
        BmpSegmentHeader seg;
        is >> ghdr.m_prim_hdr;
        is >> seg;

        for (int seg_cnt = 1; !seg.is_null(); seg_cnt++) {
            ghdr.m_bmp_seg_hdrs.push_back(seg);
            if (seg_cnt + 1 > config::MAX_BMP_SEGMENTS)
                throw std::runtime_error("MAX_BMP_SEGMENTS limit exceeded");

            is >> seg;
        }
        ghdr.m_bmp_seg_hdrs.shrink_to_fit();
        return is;
    }

    BasicGraphicHeaders::BasicGraphicHeaders(std::istream &is)
    {
        is >> *this;
    }

    std::pair<int, int> BasicGraphicHeaders::calc_dimensions() const
    {
        if (m_bmp_seg_hdrs.empty())
            throw std::runtime_error(err::UNINITIALIZED_STRUCT);

        auto wmm = std::max_element(m_bmp_seg_hdrs.begin(), m_bmp_seg_hdrs.end() - 1,
                                    [](auto &a, auto &b) {
                                        return a.m_width + a.m_rel_position_x < b.m_width + b.m_rel_position_x;
                                    });

        auto hmm = std::max_element(m_bmp_seg_hdrs.begin(), m_bmp_seg_hdrs.end() - 1,
                                    [](auto &a, auto &b) {
                                        return a.m_height + a.m_rel_position_y < b.m_height + b.m_rel_position_y;
                                    });

        int w = wmm->m_rel_position_x + wmm->m_width;
        int h = hmm->m_rel_position_y + hmm->m_height;

        if (w < 0 || h < 0)
            throw std::runtime_error(err::UNEXPECTED_NEGATIVE_RESULT);

        return {w, h};
    }

    size_t BasicGraphicHeaders::calc_bitmap_size() const
    {
        auto sum = std::accumulate(m_bmp_seg_hdrs.begin(), m_bmp_seg_hdrs.end(), 0, [&](auto acc, auto &seg) {
            return acc + seg.m_width * seg.m_height;
        });
        switch (get_bpp()) {
            case 4:
                return sum / 2;
            case 16:
                return sum * 2;
            default:
                return sum;
        }
    }

}