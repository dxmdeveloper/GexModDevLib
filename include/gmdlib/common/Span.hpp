// header only
#pragma once


#if __cplusplus >= 202002L

#include <span>
    namespace gmdlib
    {
        template<typename T>
        using Span = std::span<T>;
    }
#else
    #define nonstd _span_lite
#include <span_lite/span.hpp>
#undef nonstd

    namespace gmdlib
    {
    template<typename T>
    using Span = _span_lite::span<T>;
    }
#endif
