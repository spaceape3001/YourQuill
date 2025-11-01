////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/root/Root.hpp>
#include <yq/mithril/root/RootCDB.hpp>
#include <yq/meta/TypeMetaWriter.hpp>
#include <yq/meta/Init.hpp>

YQ_TYPE_IMPLEMENT(yq::mithril::Root)

namespace yq::mithril {
    void reg_root_meta()
    {
        {
            auto w  = writer<Root>();
            w.property("id", READ_ONLY, &Root::id);
            w.property("key", (std::string(*)(Root)) cdb::key);
        }
    }

    YQ_INVOKE(reg_root_meta();)
}
