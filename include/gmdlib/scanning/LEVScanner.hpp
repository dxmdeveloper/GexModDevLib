#include <gmdlib/helpers/BinaryStreamReader.hpp>
#include "IMemStream.hpp"
#include "LevFileChunk.hpp"

using gexptr = uint32_t;

namespace gmdlib::scanning
{
    class LEVScanner : protected bin::le::BinaryStreamReader
    {
    public:
        uint32_t gexptr_to_offset(gexptr ptr) const;

        // constructors
        LEVScanner(const std::string& filename);

        LEVScanner(IMemStream &stream);

    private:
        std::vector<LevFileChunk> chunks;
    };

    using IDLScanner = LEVScanner;
}