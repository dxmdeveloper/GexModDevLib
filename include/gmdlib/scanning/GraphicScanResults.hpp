#pragma once

#include <array>
#include <memory>
#include <gmdlib/graphics/gex/GraphicInfo.hpp>
#include <gmdlib/graphics/gex/Graphic.hpp>

namespace gmdlib::scanning
{

    class GraphicScanResults
    {
        std::array<std::vector<graphics::gex::GraphicInfo*>, graphics::gex::GRAPHIC_CATEGORIES_CNT> m_infos;
        int m_count_of_errors = 0;

    public:
        std::unique_ptr<graphics::gex::Graphic> get_obj_graphic_at(graphics::gex::GraphicCategory category,
                                                                   uint obj_ind, uint anim_set, uint anim_frame,
                                                                   uint component_ind);

        std::unique_ptr<graphics::gex::Graphic> get_tile_graphic_at(graphics::gex::GraphicCategory category,
                                                                    uint tile_ind, uint anim_frame);
    };
}