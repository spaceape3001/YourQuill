////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace yq::mithril {
    namespace kernel {
        //! The compiled build directory path
        const char*     build_root();

        //! The share path to the one inside the kernel's build directory;
        const char*     share_directories();
    }
}
