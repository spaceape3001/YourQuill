////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "FragmentCDB.hpp"
#include <yq/meta/TypeMetaWriter.hpp>
#include <yq/meta/Init.hpp>

YQ_TYPE_IMPLEMENT(yq::mithril::Fragment)
YQ_TYPE_IMPLEMENT(yq::mithril::FragmentSet)
YQ_TYPE_IMPLEMENT(yq::mithril::FragmentVector)

namespace yq::mithril {
    void reg_fragment_meta()
    {
        {
            auto w  = writer<Fragment>();
            w.property("id", READ_ONLY, &Fragment::id);
            w.property("key", (std::string(*)(Fragment)) cdb::key);
        }
    }
    YQ_INVOKE(reg_fragment_meta();)
}
