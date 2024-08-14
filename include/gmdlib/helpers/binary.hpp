#pragma once
#include <cstdint>

#ifdef BIG_ENDIAN_ARCH
#if defined(_MSC_VER)
#define bswap32(x) _byteswap_ulong(x)
#define bswap16(x) _byteswap_ushort(x)
#elif (defined(__clang__) && __has_builtin(__builtin_bswap16)) || \
    (defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 8)))
#define bswap32(x) __builtin_bswap32(x)
#define bswap16(x) __builtin_bswap16(x)
#else
inline static uint32_t __file_ex_bswap32(const uint32_t x)
{
    uint8_t tmp[4];
    tmp[0] = (uint8_t)(x >> 0);
    tmp[1] = (uint8_t)(x >> 8);
    tmp[2] = (uint8_t)(x >> 16);
    tmp[3] = (uint8_t)(x >> 24);
    return *(uint32_t *)&tmp;
}
inline static uint16_t __file_ex_bswap16(const uint16_t x)
{
    u8 tmp[2];
    tmp[0] = (uint8_t)(x >> 0);
    tmp[1] = (uint8_t)(x >> 8);
    return *(uint16_t *)&tmp;
}
#define bswap32(x) __file_ex_bswap32(x)
#define bswap16(x) __file_ex_bswap16(x)
#endif
#endif

namespace gmdlib::bin::le
{

// Read an 8-bit unsigned integer from binary data
inline uint8_t read_u8(const void *value)
{
    return *static_cast<const uint8_t *>(value);
}

// Read an 8-bit signed integer from binary data
inline int8_t read_i8(const void *value)
{
    return *static_cast<const int8_t *>(value);
}

inline uint32_t read_u32(const void *src)
{
    uint32_t val = *(uint32_t *) src;
#ifdef BIG_ENDIAN_ARCH
    val = bswap32(val);
#endif
    return val;
}

inline int32_t read_i32(const void *src)
{
    int32_t val = *(int32_t *) src;
#ifdef BIG_ENDIAN_ARCH
    val = bswap32(*(uint32_t *)&val);
#endif
    return (int32_t) val;
}

inline uint16_t read_u16(const void *src)
{
    uint16_t val = *(uint16_t *) src;
#ifdef BIG_ENDIAN_ARCH
    val = bswap16(val);
#endif
    return val;
}

inline int16_t read_i16(const void *src)
{
    int16_t val = *(int16_t *) src;
#ifdef BIG_ENDIAN_ARCH
    val = bswap16(*(uint16_t *)&val);
#endif
    return (int16_t) val;
}

inline uint32_t read_u32(const uint8_t *src)
{
    return read_u32((const void *) src);
}

inline int32_t read_i32(const uint8_t *src)
{
    return read_i32((const void *) src);
}

inline uint16_t read_u16(const uint8_t *src)
{
    return read_u16((const void *) src);
}

inline int16_t read_i16(const uint8_t *src)
{
    return read_i16((const void *) src);
}

inline uint8_t read_u8(const uint8_t *src){
    return *reinterpret_cast<const uint8_t*>(src);
}

inline int8_t read_i8(const uint8_t *src){
    return *reinterpret_cast<const int8_t*>(src);
}

}