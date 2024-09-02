////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/meta/TypeInfoWriter.hpp>
#include "FolderCDB.hpp"

namespace {
    void reg_folder_meta()
    {
        using namespace yq;
        using namespace yq::mithril;
        
        {
            auto w  = writer<Folder>();
            w.property("id", READ_ONLY, &Folder::id);
            w.property("key", (std::string(*)(Folder)) cdb::key);
        }
    }
}
