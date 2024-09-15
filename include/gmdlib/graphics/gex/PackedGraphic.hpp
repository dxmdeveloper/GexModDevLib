#pragma once

#include "Graphic.hpp"

#include <utility>
#include "PackedGraphicHeaders.hpp"
#include "BasicGraphic.hpp"

namespace gmdlib::graphics::gex
{

    /// @brief graphic compressed with RLE (something similar to PackBits is used)
    class PackedGraphic : public gex::Graphic
    {
    private:
        PackedGraphicHeaders m_headers;
        std::vector<uint8_t> m_bitmap;

    public:
        Image draw() const override;

        PackedGraphic() = default;

        PackedGraphic(PackedGraphicHeaders hdrs, Span<const uint8_t> bmp_bin,
                      std::shared_ptr<PaletteBGR555> pal = nullptr);

        PackedGraphic(PackedGraphicHeaders hdrs, Span<const uint8_t> bmp_bin, PaletteBGR555 pal);

        PackedGraphic(std::istream &is, PaletteBGR555 pal);

    private:
        std::reference_wrapper<const BasicGraphicHeaders> get_headers() const override;

        void init_bitmap(Span<const uint8_t> bmp_bin);

        template<int bpp>
        Image draw_body() const;
    };

}