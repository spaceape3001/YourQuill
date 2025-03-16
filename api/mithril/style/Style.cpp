////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/style/StyleCDB.hpp>
#include <yq/meta/TypeInfoWriter.hpp>
#include <yq/meta/Init.hpp>

YQ_TYPE_IMPLEMENT(yq::mithril::Style)
YQ_TYPE_IMPLEMENT(yq::mithril::StyleSet)
YQ_TYPE_IMPLEMENT(yq::mithril::StyleVector)

namespace yq::mithril {
    void reg_style_meta()
    {
        {
            auto w  = writer<Style>();
            w.property("id", READ_ONLY, &Style::id);
            w.property("key", (std::string(*)(Style)) cdb::key);
        }
    }

    YQ_INVOKE(reg_style_meta();)
}
