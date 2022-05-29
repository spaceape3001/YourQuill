////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "DirConfig.hpp"

namespace yq {
    namespace build {
        const char*     build_root()
        {
            return YQ_BUILD_ROOT;
        }

        const char*     share_directory()
        {
            return YQ_SHARE_DIR;
        }

        //! The share path to the one inside the build directory;
        const char*     data_directory()
        {
            return YQ_DATA_DIR;
        }
    }
}
