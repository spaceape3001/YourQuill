////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <util/String.hpp>
#include <memory>

struct TagData {

    using Shared    = std::shared_ptr<TagData>;

    String      leaf;
    String      name;
    String      brief;
    String      notes;

    
    /*! \brief Merges two tag documents
    
        \param[in] fOverride  TRUE to allo for contents of new to clobber old; otherwise, won't clobber non-empty fields.
    */
    TagData&        merge(const TagData&, bool fOverride=false);
    void            reset();
};
