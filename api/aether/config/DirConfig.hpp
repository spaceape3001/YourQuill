////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace yq {

    namespace build {

        //! The compiled build directory path
        const char*     build_root();

        //! The share path to the one inside the build directory;
        const char*     share_directory();

        //! The share path to the one inside the build directory;
        const char*     data_directory();
    
    }
}
