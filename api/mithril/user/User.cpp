////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "UserCDB.hpp"
#include <yq/meta/TypeInfoWriter.hpp>
#include <yq/meta/Init.hpp>

YQ_TYPE_IMPLEMENT(yq::mithril::User)
YQ_TYPE_IMPLEMENT(yq::mithril::UserSet)
YQ_TYPE_IMPLEMENT(yq::mithril::UserVector)

namespace yq::mithril {
    void reg_user_meta()
    {
        {
            auto w  = writer<User>();
            w.property("id", READ_ONLY, &User::id);
            w.property("key", (std::string(*)(User)) cdb::key);
        }
    }

    YQ_INVOKE(reg_user_meta();)
}
