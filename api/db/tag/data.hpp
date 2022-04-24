////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "struct.hpp"

namespace yq {

    struct Tag::Data {

        std::string      leaf;
        std::string      name;
        std::string      brief;
        std::string      notes;

        /*! \brief Merges two tag documents
        
            \param[in] fOverride  TRUE to allo for contents of new to clobber old; otherwise, won't clobber non-empty fields.
        */
        Data&       merge(const Data&, bool fOverride=false);
        void        reset();
    };
    
}
