////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "OrganizationCDB.hpp"
#include <yq/meta/TypeInfoWriter.hpp>
#include <yq/meta/Init.hpp>

YQ_TYPE_IMPLEMENT(yq::mithril::Organization)
YQ_TYPE_IMPLEMENT(yq::mithril::OrganizationSet)
YQ_TYPE_IMPLEMENT(yq::mithril::OrganizationVector)

namespace yq::mithril {
    void reg_organization_meta()
    {
        {
            auto w  = writer<Organization>();
            w.property("id", READ_ONLY, &Organization::id);
            w.property("key", (std::string(*)(Organization)) cdb::key);
        }
    }

    YQ_INVOKE(reg_organization_meta();)
}
