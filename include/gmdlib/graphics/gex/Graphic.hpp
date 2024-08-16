#pragma once

#include <gmdlib/graphics/Image.hpp>
#include "PaletteBGR555.hpp"
#include <optional>

namespace gmdlib::gfx::gex
{
    class Graphic
    {
    public:
        virtual Image draw() const = 0;
        void set_palette(std::shared_ptr<PaletteBGR555> &&pal);
        void set_palette(std::shared_ptr<PaletteBGR555> &pal);
        void set_palette(PaletteBGR555 &&pal);
        void set_palette(PaletteBGR555 &pal);

        Graphic() = default;
        explicit Graphic(std::shared_ptr<PaletteBGR555> &&pal);
        explicit Graphic(std::shared_ptr<PaletteBGR555> &pal);
        explicit Graphic(PaletteBGR555 &&pal);
        explicit Graphic(PaletteBGR555 &pal);

    private:
        std::shared_ptr<PaletteBGR555> palette = nullptr;
    };
}
