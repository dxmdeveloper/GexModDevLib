#include "gmdlib/scanning/LEVScanner.hpp"

namespace gmdlib::scanning
{

    uint32_t LEVScanner::gexptr_to_offset(gexptr ptr, uint chunk_ind) const
    {
        auto &ch = chunks.at(chunk_ind);
        return ch.gexptr_to_offset(ptr);
    }
}