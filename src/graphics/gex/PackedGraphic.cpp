#include "gmdlib/graphics/gex/PackedGraphic.hpp"
#include <gmdlib/common/common.hpp>

namespace gmdlib::gfx::gex
{
    Image PackedGraphic::draw()
    {
        switch(headers.get_bpp()){
            case 4:
                return draw_body<4>();
            case 8:
                return draw_body<8>();
            default:
                throw std::runtime_error(err::UNEXPECTED_VALUE);
        }
    }

    PackedGraphic::PackedGraphic(PackedGraphicHeaders hdrs, std::span<const uint8_t> bmp_bin,
                                 std::shared_ptr<PaletteBGR555> pal) : Graphic(std::move(pal)), headers{std::move(hdrs)}
    {
        init_bitmap(bmp_bin);
    }

    PackedGraphic::PackedGraphic(PackedGraphicHeaders hdrs, std::span<const uint8_t> bmp_bin, PaletteBGR555 pal)
            : Graphic(std::move(pal)), headers{std::move(hdrs)}
    {
        init_bitmap(bmp_bin);
    }

    void PackedGraphic::init_bitmap(std::span<const uint8_t> bmp_bin)
    {
        auto bmp_size = uint32_t(headers.calc_bitmap_size());

        if(bmp_size > config::MAX_BITMAP_BINARY_SIZE)
            throw std::runtime_error(err::BITMAP_BIN_SIZE_LIMIT_EXCEEDED);

        if(bmp_bin.size() < bmp_size)
            throw std::runtime_error(err::UNEXPECTED_END_OF_DATA);

        bitmap.resize(bmp_size);
        std::copy(bmp_bin.begin(), bmp_bin.begin() + bmp_size, bitmap.data());
    }

    template<int bpp>
    Image PackedGraphic::draw_body()
    {
        throw std::runtime_error(err::NOT_IMPLEMENTED);
    }
}