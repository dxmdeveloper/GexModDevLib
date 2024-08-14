#pragma once

#include "BasicGraphicHeaders.hpp"
#include <gmdlib/helpers/binary.hpp>
#include <gmdlib/common/common.hpp>

namespace gmdlib::gfx::gex
{

/// @brief Headers of packed graphic (lightly compressed).
    struct PackedGraphicHeaders : public BasicGraphicHeaders
    {
        // structure extends BasicGraphicHeaders with additional properties
        // uint32_t unpacking_process_data_len; // the real structure consist this field but the vector below has size in it
        std::vector<uint8_t> unpacking_process_data;

    public:
        PackedGraphicHeaders() = default;
        explicit PackedGraphicHeaders(std::istream &is);
        explicit PackedGraphicHeaders(std::span<const uint8_t> bin);

        friend std::istream &operator>>(std::istream &is, PackedGraphicHeaders &phdrs);
    };
}