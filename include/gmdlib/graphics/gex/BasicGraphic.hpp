#pragma once

#include "Graphic.hpp"
#include "BasicGraphicHeaders.hpp"

namespace gmdlib::graphics::gex
{

    class BasicGraphic : public gex::Graphic
    {
    private:
        BasicGraphicHeaders headers;
        std::vector<uint8_t> bitmap;

    public:
        Image draw() const override;

        // constructors
        BasicGraphic(Span<const uint8_t> bin, std::shared_ptr<PaletteBGR555> pal = nullptr);

        BasicGraphic(Span<const uint8_t> bin, PaletteBGR555 pal);

        BasicGraphic(Span<const uint8_t> headers_bin, Span<const uint8_t> bitmap_bin,
                     std::shared_ptr<PaletteBGR555> pal = nullptr);

        BasicGraphic(Span<const uint8_t> headers_bin, Span<const uint8_t> bitmap_bin, PaletteBGR555 pal);

        BasicGraphic(std::istream &stream, std::shared_ptr<PaletteBGR555> pal = nullptr);

        BasicGraphic(std::istream &stream, PaletteBGR555 pal);

        BasicGraphic(std::istream &stream, uint32_t bitmap_abs_offset, std::shared_ptr<PaletteBGR555> pal = nullptr);

        BasicGraphic(std::istream &stream, uint32_t bitmap_abs_offset, PaletteBGR555 pal);

        BasicGraphic(std::istream &header_stream, std::istream &bitmap_stream,
                     std::shared_ptr<PaletteBGR555> pal = nullptr);

        BasicGraphic(std::istream &header_stream, std::istream &bitmap_stream, PaletteBGR555 pal);

    private:
        template<int bpp>
        Image draw_body() const;

        std::reference_wrapper<const BasicGraphicHeaders> get_headers() const override;

        void init_bitmap_from_span(Span<const uint8_t> data);

        void init_bitmap_from_istream(std::istream &stream);
    };


}