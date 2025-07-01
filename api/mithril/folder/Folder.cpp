////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "FolderCDB.hpp"
#include <yq/meta/TypeMetaWriter.hpp>
#include <yq/meta/Init.hpp>

YQ_TYPE_IMPLEMENT(yq::mithril::Folder)
YQ_TYPE_IMPLEMENT(yq::mithril::FolderSet)
YQ_TYPE_IMPLEMENT(yq::mithril::FolderVector)

namespace yq::mithril {
    void reg_folder_meta()
    {
        {
            auto w  = writer<Folder>();
            w.property("id", READ_ONLY, &Folder::id);
            w.property("key", (std::string(*)(Folder)) cdb::key);
        }
    }
    YQ_INVOKE(reg_folder_meta();)
}
