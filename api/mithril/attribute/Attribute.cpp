////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "AttributeCDB.hpp"
#include <yq/core/DelayInit.hpp>
#include <yq/meta/TypeInfoWriter.hpp>
#include <yq/meta/Init.hpp>

namespace yq::mithril {
    void reg_attribute_meta()
    {
        {
            auto w  = writer<Attribute>();
            w.property("children", (AttributeVector(*)(Attribute)) cdb::child_attributes);
            w.property("document", (Document(*)(Attribute)) cdb::document);
            w.property("index", (uint64_t(*)(Attribute)) cdb::index);
            w.property("id", READ_ONLY, &Attribute::id);
            w.property("key", (std::string(*)(Attribute)) cdb::key);
            w.property("name", (std::string(*)(Attribute)) cdb::name);
            w.property("parent", (Attribute(*)(Attribute)) cdb::parent);
            w.property("uid", (uint64_t(*)(Attribute)) cdb::uid);
            w.property("value", (std::string(*)(Attribute)) cdb::value);
        }
    }
    
    YQ_INVOKE(reg_attribute_meta();)
}

YQ_TYPE_IMPLEMENT(yq::mithril::Attribute)
YQ_TYPE_IMPLEMENT(yq::mithril::AttributeSet)
YQ_TYPE_IMPLEMENT(yq::mithril::AttributeVector)
