#pragma once

#include <gmdlib/graphics/Image.hpp>
#include <gmdlib/graphics/gex/BasicGraphicHeaders.hpp>
#include "PaletteBGR555.hpp"
#include <optional>

namespace gmdlib::graphics::gex
{
    class Graphic
    {
    public:
        virtual Image draw() const = 0;

        std::pair<int, int> calc_dimensions() const
        {
            return get_headers().get().calc_dimensions();
        };
        void set_palette(std::shared_ptr<PaletteBGR555> &&pal);
        void set_palette(std::shared_ptr<PaletteBGR555> &pal);
        void set_palette(PaletteBGR555 &&pal);
        void set_palette(PaletteBGR555 &pal);

    protected:
        template<int bpp>
        void draw_body_pre_validate() const;

        virtual std::reference_wrapper<const BasicGraphicHeaders> get_headers() const = 0;

        // constructors
        Graphic() = default;
        explicit Graphic(std::shared_ptr<PaletteBGR555> &&pal);
        explicit Graphic(std::shared_ptr<PaletteBGR555> &pal);
        explicit Graphic(PaletteBGR555 &&pal);
        explicit Graphic(PaletteBGR555 &pal);

    protected:
        std::shared_ptr<PaletteBGR555> m_palette = nullptr;
    };
}
