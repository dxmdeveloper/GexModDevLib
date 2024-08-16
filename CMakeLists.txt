cmake_minimum_required(VERSION 3.20)
project(GexModLibCpp)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_compile_options(-gdwarf-4)

add_executable(GexModLibCpp
        tests/main.cpp
        include/gmdlib/graphics/gex/BmpSegmentHeader.hpp
        src/graphics/gex/BmpSegmentHeader.cpp
        include/gmdlib/helpers/binary.hpp
        include/gmdlib/graphics.hpp
        src/graphics/gex/PrimaryGraphicHeader.cpp
        include/gmdlib/graphics/gex/BasicGraphicHeaders.hpp
        src/graphics/gex/BasicGraphicHeaders.cpp
        src/graphics/gex/BasicGraphicHeaders.cpp
        include/_gmdlib_config.hpp
        include/gmdlib/helpers/BinaryStreamReader.hpp
        include/gmdlib/graphics/gex/PackedGraphicHeaders.hpp
        include/gmdlib/common/common.hpp
        include/gmdlib/common/err_messages.hpp
        src/graphics/gex/PackedGraphicHeaders.cpp
        include/gmdlib/graphics/gex/Graphic.hpp
        include/gmdlib/graphics/gex/BasicGraphic.hpp
        include/gmdlib/graphics/gex/PackedGraphic.hpp
        include/gmdlib/graphics/Image.hpp
        include/gmdlib/graphics/Color.hpp
        src/graphics/Image.cpp
        include/gmdlib/graphics/gex/PaletteBGR555.hpp
        src/graphics/gex/PaletteBGR555.cpp
        src/graphics/gex/Graphic.cpp)
