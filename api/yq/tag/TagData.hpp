////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <yq/preamble.hpp>
#include <memory>

namespace yq {

    struct TagData {

        using Shared    = std::shared_ptr<TagData>;

        std::string      leaf;
        std::string      name;
        std::string      brief;
        std::string      notes;

        
        /*! \brief Merges two tag documents
        
            \param[in] fOverride  TRUE to allo for contents of new to clobber old; otherwise, won't clobber non-empty fields.
        */
        TagData&        merge(const TagData&, bool fOverride=false);
        void            reset();
    };
}
