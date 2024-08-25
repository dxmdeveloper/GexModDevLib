#include "gmdlib/graphics/gex/PackedGraphic.hpp"
#include <gmdlib/common/common.hpp>

enum class UnpackingOperation
{
    draw_n = 0, repeat_4_bytes = 1
};

namespace gmdlib::gfx::gex
{
    Image PackedGraphic::draw() const
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

    PackedGraphic::PackedGraphic(std::istream &is, PaletteBGR555 pal) : Graphic{std::move(pal)}
    {
        is >> headers;

        bitmap = std::vector<uint8_t>(headers.calc_bitmap_size());
        is.read((char *) bitmap.data(), bitmap.size());
    }

    template<int bpp>
    Image PackedGraphic::draw_body() const
    {
        static_assert(bpp == 4 || bpp == 8);
        if (palette == nullptr)
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

        auto next_seg = [&]() {
            bmp_seg++;
            // pix_ind = bmp_seg->start_offset - hdrs_size;
            seg_pix_ind = 0;
        };

        for (uint8_t unpacking_inst: headers.unpacking_process_data) {
            if (bmp_seg->is_null()) break;

            auto operation = UnpackingOperation(!!(unpacking_inst & 0x80));
            uint8_t unpack_val = unpacking_inst & 0x7F;

            int repeats = (unpack_val == 0 ? 512 : unpack_val * 4);
            for (int r = 0; r < repeats; r++) {
                uint bmp_ind{};

                // lambdas to calculate x and y depending on the segment pixel index and position of the segment
                auto get_x = [&](uint seg_pix) {
                    return (seg_pix % bmp_seg->width) + bmp_seg->rel_position_x;
                };
                auto get_y = [&](uint seg_pix) {
                    return (seg_pix / bmp_seg->width) + bmp_seg->rel_position_y;
                };

                // switch to next segment if previous was filled
                // the second condition shouldn't happen, but should be handled
                if (seg_pix_ind / bmp_seg->width >= bmp_seg->height ||
                    (bpp == 4 && (seg_pix_ind + 1) / bmp_seg->width == bmp_seg->height)) {
                    next_seg();
                    if (bmp_seg->is_null()) break;
                }

                // update pixel index based on operation type and unpacking value
                switch (operation) {
                    case UnpackingOperation::draw_n :
                        bmp_ind = pix_ind++;
                        break;
                    case UnpackingOperation::repeat_4_bytes :
                        bmp_ind = pix_ind + (r % 4);
                        break;
                }

                // draw pixels
                if constexpr (bpp == 8) {
                    img.set_pixel(ColorRGBA(palette->at(bitmap[bmp_ind])), get_x(seg_pix_ind), get_y(seg_pix_ind));
                } else {
                    img.set_pixel(ColorRGBA(palette->at(bitmap[bmp_ind] & 0x0F)), get_x(seg_pix_ind), get_y(seg_pix_ind));
                    img.set_pixel(ColorRGBA(palette->at(bitmap[bmp_ind] >> 4)), get_x(seg_pix_ind + 1), get_y(seg_pix_ind + 1));
                }

                seg_pix_ind += 8 / bpp;
            }

            if (operation == UnpackingOperation::repeat_4_bytes) {
                pix_ind += 4;
            }
        }

        return img;
    }
}