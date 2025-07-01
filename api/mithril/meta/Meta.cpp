////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "MetaCDB.hpp"

#include <yq/meta/TypeMetaWriter.hpp>
#include <yq/meta/Init.hpp>

YQ_TYPE_IMPLEMENT(yq::mithril::Meta)

namespace yq::mithril {
    void reg_meta_meta()
    {
        {
            auto w  = writer<yq::mithril::Meta>();
            w.property("id", READ_ONLY, &yq::mithril::Meta::id);
            w.property("key", (std::string(*)(yq::mithril::Meta)) cdb::key);
        }
    }
    
    YQ_INVOKE(reg_meta_meta();)
}
