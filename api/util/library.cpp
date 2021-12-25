////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "preamble.hpp"
#include <atomic>

namespace yq {
    namespace thread {
        unsigned int id()
        {
            static std::atomic<unsigned int>    sNext{0};
            static thread_local unsigned int    sRet    = sNext++;
            return sRet;
        }
    }

    const char*     build_directory()
    {
        return YQ_BUILD_ROOT;
    }

    const char*     share_directory()
    {
        return YQ_SHARE_DIR;
    }
}
