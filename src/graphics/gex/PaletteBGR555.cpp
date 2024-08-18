#include "gmdlib/graphics/gex/PaletteBGR555.hpp"
#include <gmdlib/common/common.hpp>
#include <gmdlib/helpers/BinaryStreamReader.hpp>

namespace gmdlib::gfx::gex
{
    static int get_color_cnt(uint8_t raw_val);

    size_t PaletteBGR555::size() const
    {
        return colors.size();
    }

    PaletteBGR555::PaletteBGR555(std::span<uint8_t> bin)
    {
        if (bin.size() < 4)
            throw std::runtime_error(err::UNEXPECTED_END_OF_DATA);

        uint8_t raw_c_cnt = bin.data()[0];
        auto c_cnt = get_color_cnt(raw_c_cnt);

        if (bin.size() < c_cnt + 4)
            throw std::runtime_error(err::UNEXPECTED_END_OF_DATA);

        colors = std::vector<BGR555>(c_cnt);
        for (int i = 0; i < c_cnt; i++) {
            colors[i] = bin::le::read_u16(bin.data() + 4 + (i * sizeof(BGR555)));
        }
    }

    PaletteBGR555::PaletteBGR555(std::istream &is)
    {
        is >> *this;
    }

    std::istream &operator>>(std::istream &is, PaletteBGR555 &pal)
    {
        bin::le::BinaryStreamReader reader(&is);
        uint8_t c_cnt{};

        is >> c_cnt;
        is.ignore(3);

        pal.colors = std::vector<BGR555>(get_color_cnt(c_cnt));
        for (auto &color: pal.colors) {
            reader >> color;
        }
        return is;
    }

    static int get_color_cnt(uint8_t raw_val)
    {
        switch (raw_val) {
            case 0:
                return 16;
            case 1:
                return 256;
            case 2:
                return 0;
            default:
                throw std::runtime_error(err::UNEXPECTED_VALUE);
        }
    }

}
