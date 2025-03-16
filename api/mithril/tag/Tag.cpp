////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "TagCDB.hpp"
#include <yq/meta/TypeInfoWriter.hpp>
#include <yq/meta/Init.hpp>

YQ_TYPE_IMPLEMENT(yq::mithril::Tag)
YQ_TYPE_IMPLEMENT(yq::mithril::TagSet)
YQ_TYPE_IMPLEMENT(yq::mithril::TagVector)

namespace yq::mithril {
    void reg_tag_meta()
    {
        {
            auto w  = writer<Tag>();
            w.property("id", READ_ONLY, &Tag::id);
            w.property("key", (std::string(*)(Tag)) cdb::key);
        }
    }

    YQ_INVOKE(reg_tag_meta();)
}
