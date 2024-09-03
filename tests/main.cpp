#include <iostream>
#include "gmdlib/graphics.hpp"
#include "gmdlib/graphics/gex/PackedGraphic.hpp"
#include <gmdlib/helpers/BinaryStreamReader.hpp>
#include <fstream>
#include <format>

int main(int argc, char *argv[])
{
    std::ifstream graphic_file("test_graphic.bin", std::ios::binary);
    std::ifstream palette_file("test_palette.bin", std::ios::binary);

    gmdlib::graphics::gex::PaletteBGR555 palette(palette_file);
    gmdlib::graphics::gex::BasicGraphic graphic(graphic_file, palette);
    auto img = graphic.draw();

    std::ofstream output("output.png", std::ios::binary);
    auto png_dat = img.to_png();

    for(auto b : png_dat) {
        output << b;
    }
    output.close();

    return 0; // for breakpoint
}