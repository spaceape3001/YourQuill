////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "GroupCDB.hpp"
#include <yq/meta/TypeMetaWriter.hpp>
#include <yq/meta/Init.hpp>

YQ_TYPE_IMPLEMENT(yq::mithril::Group)
YQ_TYPE_IMPLEMENT(yq::mithril::GroupSet)
YQ_TYPE_IMPLEMENT(yq::mithril::GroupVector)

namespace yq::mithril {
    void reg_group_meta()
    {
        {
            auto w  = writer<Group>();
            w.property("id", READ_ONLY, &Group::id);
            w.property("key", (std::string(*)(Group)) cdb::key);
        }
    }

    YQ_INVOKE(reg_group_meta();)
}
