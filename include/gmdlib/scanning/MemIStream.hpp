#include <istream>
#include <cstdint>
#include <gmdlib/common/Span.hpp>

namespace gmdlib::scanning
{
    // https://stackoverflow.com/questions/7781898/get-an-istream-from-a-char
    // https://stackoverflow.com/questions/13059091/creating-an-input-stream-from-constant-memory
    struct MemBuf : std::streambuf
    {
        MemBuf(const uint8_t *base, size_t size)
        {
            auto p(const_cast<char *>((char *) base));
            this->setg(p, p, p + size);
        }

        pos_type seekoff(off_type off, std::ios_base::seekdir dir, std::ios_base::openmode which) override
        {
            if (dir == std::ios_base::cur)
                gbump(off);
            else if (dir == std::ios_base::end)
                setg(eback(), egptr() + off, egptr());
            else if (dir == std::ios_base::beg)
                setg(eback(), eback() + off, egptr());
            return gptr() - eback();
        }

        pos_type seekpos(pos_type sp, std::ios_base::openmode which) override
        {
            return seekoff(sp - pos_type(off_type(0)), std::ios_base::beg, which);
        }
    };

    struct MemIStream : virtual MemBuf, std::istream
    {
        MemIStream(const uint8_t *bin, size_t size)
                : MemBuf(bin, size), std::istream(static_cast<std::streambuf *>(this))
        {
        }

        MemIStream(Span<const uint8_t> bin)
                : MemBuf(bin.data(), bin.size()), std::istream(static_cast<std::streambuf *>(this))
        {
        }
    };
}