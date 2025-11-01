////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "LeafCDB.hpp"
#include <yq/meta/TypeMetaWriter.hpp>
#include <yq/meta/Init.hpp>
#include <mithril/notify/Notifier.hxx>

YQ_TYPE_IMPLEMENT(yq::mithril::Leaf)
YQ_TYPE_IMPLEMENT(yq::mithril::LeafSet)
YQ_TYPE_IMPLEMENT(yq::mithril::LeafVector)

namespace yq::mithril {
    void reg_leaf_meta()
    {
        {
            auto w  = writer<Leaf>();
            w.property("id", READ_ONLY, &Leaf::id);
            w.property("key", (std::string(*)(Leaf)) cdb::key);
        }
    }

    YQ_INVOKE(reg_leaf_meta();)
    template class Notifier<const Leaf::Diff&>;
}
