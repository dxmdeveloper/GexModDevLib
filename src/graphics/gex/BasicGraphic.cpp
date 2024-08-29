#include "gmdlib/graphics/gex/BasicGraphic.hpp"
#include "gmdlib/common/common.hpp"

namespace gmdlib::graphics::gex
{
    template<>
    Image BasicGraphic::draw_body<16>() const;

    Image BasicGraphic::draw() const
    {
        switch (headers.get_bpp()) {
            case 4:
                return draw_body<4>();
            case 8:
                return draw_body<8>();
            case 16:
                return draw_body<16>();
            default:
                throw std::runtime_error(err::UNEXPECTED_VALUE);
        }
    }

    BasicGraphic::BasicGraphic(Span<const uint8_t> bin, std::shared_ptr<PaletteBGR555> pal)
            : Graphic(std::move(pal)), headers(bin)
    {
        bin = bin.subspan(headers.get_raw_size_of_headers());
        init_bitmap_from_span(bin);
    }

    BasicGraphic::BasicGraphic(Span<const uint8_t> bin, PaletteBGR555 pal)
            : BasicGraphic(bin, std::make_shared<PaletteBGR555>(std::move(pal))) {}

    BasicGraphic::BasicGraphic(Span<const uint8_t> headers_bin, Span<const uint8_t> bitmap_bin,
                               std::shared_ptr<PaletteBGR555> pal)
            : Graphic(std::move(pal)), headers(headers_bin)
    {
        init_bitmap_from_span(bitmap_bin);
    }

    BasicGraphic::BasicGraphic(Span<const uint8_t> headers_bin, Span<const uint8_t> bitmap_bin, PaletteBGR555 pal)
            : BasicGraphic(headers_bin, bitmap_bin, std::make_shared<PaletteBGR555>(std::move(pal))) {}

    BasicGraphic::BasicGraphic(std::istream &stream, std::shared_ptr<PaletteBGR555> pal)
            : Graphic(std::move(pal)), headers(stream)
    {
        init_bitmap_from_istream(stream);
    }

    BasicGraphic::BasicGraphic(std::istream &stream, PaletteBGR555 pal)
            : BasicGraphic(stream, std::make_shared<PaletteBGR555>(std::move(pal))) {}

    BasicGraphic::BasicGraphic(std::istream &stream, uint32_t bitmap_abs_offset,
                               std::shared_ptr<PaletteBGR555> pal)
            : Graphic(std::move(pal))
    {
        stream.seekg(bitmap_abs_offset, std::ios::beg);
        init_bitmap_from_istream(stream);
    }

    BasicGraphic::BasicGraphic(std::istream &stream, uint32_t bitmap_abs_offset, PaletteBGR555 pal)
            : BasicGraphic(stream, bitmap_abs_offset, std::make_shared<PaletteBGR555>(std::move(pal))) {}

    BasicGraphic::BasicGraphic(std::istream &header_stream, std::istream &bitmap_stream,
                               std::shared_ptr<PaletteBGR555> pal)
            : Graphic(std::move(pal)), headers(header_stream)
    {
        init_bitmap_from_istream(bitmap_stream);
    }

    BasicGraphic::BasicGraphic(std::istream &header_stream, std::istream &bitmap_stream, PaletteBGR555 pal)
            : BasicGraphic(header_stream, bitmap_stream, std::make_shared<PaletteBGR555>(std::move(pal))) {}


    template<int bpp>
    Image BasicGraphic::draw_body() const
    {
        static_assert(bpp == 4 || bpp == 8);
        draw_body_pre_validate<bpp>();

        auto [width, height] = calc_dimensions();
        Image img(width, height);

        int pix_ind = 0;

        for (auto &seg: headers.bmp_seg_hdrs) {
            auto seg_pix_cnt = seg.width * seg.height;
            for (int i = 0; i < seg_pix_cnt; i++) {
                int x = seg.rel_position_x + pix_ind % seg.width;
                int y = seg.rel_position_y + pix_ind / seg.width;

                uint8_t pix_val = bitmap.at(pix_ind / (8 / bpp));
                if constexpr (bpp == 4)
                    pix_val = (pix_val >> (4 * (i % 2))) & 0x0F;

                ColorRGBA color{palette->at(pix_val)};
                img.set_pixel(color, x, y);
                pix_ind++;
            }
        }
        return img;
    }

    template<>
    Image BasicGraphic::draw_body<16>() const
    {
        auto [width, height] = calc_dimensions();
        Image img(width, height);

        int pix_ind = 0;

        for (auto &seg: headers.bmp_seg_hdrs) {
            auto seg_pix_cnt = seg.width * seg.height;
            for (int i = 0; i < seg_pix_cnt; i++) {
                int x = seg.rel_position_x + pix_ind % seg.width;
                int y = seg.rel_position_y + pix_ind / seg.width;

                uint8_t pix_val = bitmap.at(pix_ind);
                img.set_pixel(ColorRGBA(pix_val++), x, y);
            }
        }
        return img;
    }

    std::reference_wrapper<const BasicGraphicHeaders> BasicGraphic::get_headers() const
    {
        return std::ref(headers);
    }

    void BasicGraphic::init_bitmap_from_span(Span<const uint8_t> data)
    {
        if (headers.prim_hdr.is_packed())
            throw std::runtime_error(err::INVALID_FORMAT_GRAPHIC_TYPE);

        auto bmp_size = headers.calc_bitmap_size();

        if (bmp_size > config::MAX_BITMAP_BINARY_SIZE)
            throw std::runtime_error(err::BITMAP_BIN_SIZE_LIMIT_EXCEEDED);
        if (bitmap.size() < bmp_size)
            throw std::runtime_error(err::UNEXPECTED_END_OF_DATA);

        bitmap.resize(bmp_size);
        std::copy(data.begin(), data.begin() + ptrdiff_t(bmp_size), bitmap.begin());
    }

    void BasicGraphic::init_bitmap_from_istream(std::istream &stream)
    {
        if (headers.prim_hdr.is_packed())
            throw std::runtime_error(err::INVALID_FORMAT_GRAPHIC_TYPE);

        auto bmp_size = this->headers.calc_bitmap_size();
        if (bmp_size > config::MAX_BITMAP_BINARY_SIZE)
            throw std::runtime_error(err::BITMAP_BIN_SIZE_LIMIT_EXCEEDED);

        bitmap.resize(bmp_size);
        stream.read((char *) (bitmap.data()), std::streamsize(bmp_size));
    }

}

