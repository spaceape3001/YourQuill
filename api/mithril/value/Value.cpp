////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "ValueCDB.hpp"
#include <yq/meta/TypeMetaWriter.hpp>
#include <yq/meta/Init.hpp>

YQ_TYPE_IMPLEMENT(yq::mithril::Value)
YQ_TYPE_IMPLEMENT(yq::mithril::ValueSet)
YQ_TYPE_IMPLEMENT(yq::mithril::ValueVector)

namespace yq::mithril {
    void reg_value_meta()
    {
        {
            auto w  = writer<Value>();
            w.property("id", READ_ONLY, &Value::id);
            w.property("key", (std::string(*)(Value)) cdb::key);
        }
    }

    YQ_INVOKE(reg_value_meta();)
}
