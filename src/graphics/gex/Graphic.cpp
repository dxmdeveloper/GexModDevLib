#include "gmdlib/graphics/gex/Graphic.hpp"

namespace gmdlib::gfx::gex
{

    void Graphic::set_palette(std::shared_ptr<PaletteBGR555> &&pal) { palette = pal; }

    void Graphic::set_palette(std::shared_ptr<PaletteBGR555> &pal) { palette = pal; }

    void Graphic::set_palette(PaletteBGR555 &&pal) { palette = std::make_shared<PaletteBGR555>(pal); }

    void Graphic::set_palette(PaletteBGR555 &pal) { palette = std::make_shared<PaletteBGR555>(pal); }

    Graphic::Graphic(std::shared_ptr<PaletteBGR555> &&pal) : palette {pal}{}

    Graphic::Graphic(std::shared_ptr<PaletteBGR555> &pal) : palette {pal}{}

    Graphic::Graphic(PaletteBGR555 &&pal) : palette {std::make_shared<PaletteBGR555>(pal)}{}

    Graphic::Graphic(PaletteBGR555 &pal) : palette {std::make_shared<PaletteBGR555>(pal)}{}
}