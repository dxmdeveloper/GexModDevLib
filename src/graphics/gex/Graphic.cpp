#include "gmdlib/graphics/gex/Graphic.hpp"
#include <gmdlib/common/common.hpp>

namespace gmdlib::graphics::gex
{

    void Graphic::set_palette(std::shared_ptr<PaletteBGR555> &&pal) { palette = pal; }

    void Graphic::set_palette(std::shared_ptr<PaletteBGR555> &pal) { palette = pal; }

    void Graphic::set_palette(PaletteBGR555 &&pal) { palette = std::make_shared<PaletteBGR555>(pal); }

    void Graphic::set_palette(PaletteBGR555 &pal) { palette = std::make_shared<PaletteBGR555>(pal); }

    Graphic::Graphic(std::shared_ptr<PaletteBGR555> &&pal) : palette{pal} {}

    Graphic::Graphic(std::shared_ptr<PaletteBGR555> &pal) : palette{pal} {}

    Graphic::Graphic(PaletteBGR555 &&pal) : palette{std::make_shared<PaletteBGR555>(pal)} {}

    Graphic::Graphic(PaletteBGR555 &pal) : palette{std::make_shared<PaletteBGR555>(pal)} {}

    template<int bpp>
    void Graphic::draw_body_pre_validate() const
    {
        static_assert(bpp == 4 || bpp == 8);
        if (palette == nullptr)
            throw std::runtime_error(err::COLOR_PALETTE_NOT_ASSIGNED);
        if ((bpp == 8 && palette->size() != 256) || (bpp == 4 && palette->size() != 16))
            throw std::runtime_error(err::COLOR_PALETTE_INCOMPATIBLE);
    }

    template void Graphic::draw_body_pre_validate<4>() const;
    template void Graphic::draw_body_pre_validate<8>() const;
}