#pragma once

#include "BasicGraphicHeaders.hpp"
#include <gmdlib/helpers/binary.hpp>
#include <gmdlib/common/common.hpp>

namespace gmdlib::graphics::gex
{

    struct PackedGraphicHeaders : public BasicGraphicHeaders
    {
        // structure extends BasicGraphicHeaders with additional properties
        // uint32_t unpacking_process_data_len; // the real structure consist this field but the vector below has m_size in it
        std::vector<uint8_t> m_unpacking_process_data;

    public:
        size_t calc_bitmap_size() const override;

        PackedGraphicHeaders() = default;
        explicit PackedGraphicHeaders(std::istream &is);
        explicit PackedGraphicHeaders(Span<const uint8_t> bin);

        friend std::istream &operator>>(std::istream &is, PackedGraphicHeaders &phdrs);
    };
}