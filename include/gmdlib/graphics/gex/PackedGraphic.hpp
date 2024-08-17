#pragma once

#include "Graphic.hpp"

#include <utility>
#include "PackedGraphicHeaders.hpp"

namespace gmdlib::gfx::gex
{

    struct PackedGraphic : public gex::Graphic
    {
        PackedGraphicHeaders headers;
        std::vector<uint8_t> bitmap;

    public:
        Image draw();

        PackedGraphic() = default;

        PackedGraphic(PackedGraphicHeaders hdrs, std::span<const uint8_t> bmp_bin,
                      std::shared_ptr<PaletteBGR555> pal = nullptr);

        PackedGraphic(PackedGraphicHeaders hdrs, std::span<const uint8_t> bmp_bin, PaletteBGR555 pal);

    private:
        void init_bitmap(std::span<const uint8_t> bmp_bin);

        template<int bpp>
        Image draw_body();
    };

}