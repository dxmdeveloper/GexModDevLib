#include "gmdlib/graphics/gex/PackedGraphic.hpp"
#include <gmdlib/common/common.hpp>

enum class UnpackingOperation
{
    draw_n = 0, repeat_4_bytes = 1
};

namespace gmdlib::gfx::gex
{
    Image PackedGraphic::draw()
    {
        switch (headers.get_bpp()) {
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

        if (bmp_size > config::MAX_BITMAP_BINARY_SIZE)
            throw std::runtime_error(err::BITMAP_BIN_SIZE_LIMIT_EXCEEDED);

        if (bmp_bin.size() < bmp_size)
            throw std::runtime_error(err::UNEXPECTED_END_OF_DATA);

        bitmap.resize(bmp_size);
        std::copy(bmp_bin.begin(), bmp_bin.begin() + bmp_size, bitmap.data());
    }

    template<int bpp>
    Image PackedGraphic::draw_body()
    {
        static_assert(bpp == 4 || bpp == 8);
        if(palette == nullptr)
            throw std::runtime_error(err::COLOR_PALETTE_NOT_ASSIGNED);
        if ((bpp == 8 && palette->size() != 256) || (bpp == 4 && palette->size() != 16))
            throw std::runtime_error(err::COLOR_PALETTE_INCOMPATIBLE);

        auto [width, height] = headers.calc_dimensions();
        Image img(width, height);
        auto bmp_seg = headers.bmp_seg_hdrs.begin();
        // size_t hdrs_size = headers.get_raw_size_of_headers();
        // int pix_ind = bmp_seg->start_offset - hdrs_size;
        // above has been commented because bitmap vector size was calculated without taking start_offset into account.
        int pix_ind = 0;
        int seg_pix_ind = 0;

        for (uint8_t unpacking_inst: headers.unpacking_process_data) {
            auto operation = UnpackingOperation(unpacking_inst & 0x80);
            uint8_t unpack_val = unpacking_inst & 0x7F;
            int repeats = (unpack_val == 0 ? 4096 : unpack_val * 32) / bpp;

            // next segment
            if (seg_pix_ind > bmp_seg->width * bmp_seg->height) {
                bmp_seg++;
                seg_pix_ind = 0;
                // pix_ind = bmp_seg->start_offset - hdrs_size;
                if (bmp_seg->is_null()) break;
            }

            for (int r = 0; r < repeats; r++) {
                uint8_t color_pal_ind{};
                int cx = seg_pix_ind % bmp_seg->width;
                int cy = seg_pix_ind / bmp_seg->width;
                if (cy >= bmp_seg->height) break; // bitmap segment overflow

                switch (operation) {
                    case UnpackingOperation::draw_n :
                        if constexpr (bpp == 8)
                            color_pal_ind = bitmap[pix_ind];
                        else
                            color_pal_ind = (bitmap[pix_ind / 2] >> pix_ind % 2) & 0x0F;

                        pix_ind++;
                        break;
                    case UnpackingOperation::repeat_4_bytes :
                        if constexpr (bpp == 8)
                            color_pal_ind = bitmap[pix_ind + (r % 4)];
                        else
                            color_pal_ind = (bitmap[(pix_ind + (r % 8)) / 2] >> (pix_ind + r) % 2) & 0x0F;
                        break;
                }

                img.set_pixel(ColorRGBA(palette->colors.at(color_pal_ind)),
                              bmp_seg->rel_position_x + cx, bmp_seg->rel_position_y + cy,
                              false);
                seg_pix_ind++;
            }
        }

        return img;
    }
}