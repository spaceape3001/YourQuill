////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "directories.hpp"

namespace yq::mithril::kernel {
    const char*     build_root()
    {
        return YQ_BUILD_ROOT;
    }

    const char*     share_directories()
    {
        return YQ_SHARE_DIRS;
    }
}
