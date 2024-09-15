cmake_minimum_required(VERSION 3.20)
project(GexModDevLib)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-gdwarf-4)

find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIRS})

include_directories(include)
include_directories(include/gmdlib/3rd_party)

set(3rd_party_sources
        # https://github.com/martinmoene/span-lite
        include/gmdlib/3rd_party/span_lite/span.hpp
)

set(lib_sources
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
        src/graphics/gex/Graphic.cpp
        src/graphics/gex/PackedGraphic.cpp
        src/graphics/gex/BasicGraphic.cpp
        include/gmdlib/common/Span.hpp
        include/gmdlib/scanning/LEVScanner.hpp
        include/gmdlib/scanning/MemIStream.hpp
        include/gmdlib/scanning/LEVFileChunkMetadata.hpp
        src/scanning/LEVFileChunkMetadata.cpp
        src/scanning/LevFileChunk.cpp
        include/gmdlib/scanning/LevFileChunk.hpp
        src/scanning/LEVScanner.cpp
)

add_executable(GexModDevLib
        ${3rd_party_sources}
        ${lib_sources}
        tests/main.cpp
)


target_link_libraries(GexModDevLib ${PNG_LIBRARY})