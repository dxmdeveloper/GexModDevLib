// header only
#pragma once

#include <iostream>
#include <gmdlib/common/Span.hpp>
#include <vector>
#include <memory>
#include "binary.hpp"

namespace gmdlib::bin::le
{

    class BinaryStreamReader
    {
    public:
        uint8_t read_u8();
        int8_t read_i8();
        uint16_t read_u16();
        int16_t read_i16();
        uint32_t read_u32();
        int32_t read_i32();
        void read(Span<uint8_t> buf, size_t n);
        std::vector<uint8_t> read(size_t n);

        // constructors
        explicit BinaryStreamReader(std::istream *observer_ptr);
        explicit BinaryStreamReader(std::unique_ptr<std::istream> is);

        // destructor
        ~BinaryStreamReader();

        // operator overloads
        BinaryStreamReader &operator>>(uint8_t &v);
        BinaryStreamReader &operator>>(int8_t &v);
        BinaryStreamReader &operator>>(uint16_t &v);
        BinaryStreamReader &operator>>(int16_t &v);
        BinaryStreamReader &operator>>(uint32_t &v);
        BinaryStreamReader &operator>>(int32_t &v);
    protected:
        std::istream *m_stream;
        bool m_stream_ownership = false;
    };

    inline BinaryStreamReader::BinaryStreamReader(std::unique_ptr<std::istream> is)
            : m_stream{is.release()}, m_stream_ownership{true} {}

    inline BinaryStreamReader::BinaryStreamReader(std::istream *observer_ptr)
            : m_stream{observer_ptr}, m_stream_ownership{false} {}

    inline BinaryStreamReader::~BinaryStreamReader()
    {
        if (m_stream_ownership)
            delete m_stream;
    }

    inline uint8_t BinaryStreamReader::read_u8()
    {
        uint8_t val;
        *m_stream >> val;
        return val;
    }

    inline int8_t BinaryStreamReader::read_i8()
    {
        int8_t val;
        *m_stream >> val;
        return val;
    }

    inline uint16_t BinaryStreamReader::read_u16()
    {
        uint16_t val;
        m_stream->read((char *) &val, 2);
        return gmdlib::bin::le::read_u16(&val);
    }

    inline int16_t BinaryStreamReader::read_i16()
    {
        int16_t val;
        m_stream->read((char *) &val, 2);
        return gmdlib::bin::le::read_i16(&val);
    }

    inline uint32_t BinaryStreamReader::read_u32()
    {
        uint32_t val;
        m_stream->read((char *) &val, 4);
        return gmdlib::bin::le::read_u32(&val);
    }

    inline int32_t BinaryStreamReader::read_i32()
    {
        int32_t val;
        m_stream->read((char *) &val, 4);
        return gmdlib::bin::le::read_i32(&val);
    }

    inline void BinaryStreamReader::read(Span<uint8_t> buf, size_t n)
    {
        if (buf.size() < n) throw std::runtime_error("buffer is too small");
        m_stream->read(reinterpret_cast<char *>(buf.data()), std::streamsize(n));
    }

    inline std::vector<uint8_t> BinaryStreamReader::read(size_t n)
    {
        std::vector<uint8_t> buf(n);
        this->read(buf, n);
        return buf;
    }

    inline BinaryStreamReader &BinaryStreamReader::operator>>(uint8_t &v)
    {
        v = this->read_u8();
        return *this;
    }

    inline BinaryStreamReader &BinaryStreamReader::operator>>(int8_t &v)
    {
        v = this->read_i8();
        return *this;
    }

    inline BinaryStreamReader &BinaryStreamReader::operator>>(uint16_t &v)
    {
        v = this->read_u16();
        return *this;
    }

    inline BinaryStreamReader &BinaryStreamReader::operator>>(int16_t &v)
    {
        v = this->read_i16();
        return *this;
    }

    inline BinaryStreamReader &BinaryStreamReader::operator>>(uint32_t &v)
    {
        v = this->read_u32();
        return *this;
    }

    inline BinaryStreamReader &BinaryStreamReader::operator>>(int32_t &v)
    {
        v = this->read_i32();
        return *this;
    }

}
