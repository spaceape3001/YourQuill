////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "NovelCDB.hpp"
#include <yq/meta/TypeInfoWriter.hpp>
#include <yq/meta/Init.hpp>
#include <mithril/notify/Notifier.hxx>

YQ_TYPE_IMPLEMENT(yq::mithril::Novel)
YQ_TYPE_IMPLEMENT(yq::mithril::NovelSet)
YQ_TYPE_IMPLEMENT(yq::mithril::NovelVector)

namespace yq::mithril {
    void reg_novel_meta()
    {
        {
            auto w  = writer<Novel>();
            w.property("id", READ_ONLY, &Novel::id);
            w.property("key", (std::string(*)(Novel)) cdb::key);
        }
    }

    YQ_INVOKE(reg_novel_meta();)
    template class Notifier<const Novel::Diff&>;
}
