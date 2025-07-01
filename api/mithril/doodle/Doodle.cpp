////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/doodle/DoodleCDB.hpp>
#include <yq/meta/TypeMetaWriter.hpp>
#include <yq/meta/Init.hpp>
#include <mithril/notify/Notifier.hxx>

YQ_TYPE_IMPLEMENT(yq::mithril::Doodle)
YQ_TYPE_IMPLEMENT(yq::mithril::DoodleSet)
YQ_TYPE_IMPLEMENT(yq::mithril::DoodleVector)

namespace yq::mithril {
    void reg_doodle_meta()
    {
        {
            auto w  = writer<Doodle>();
            w.property("id", READ_ONLY, &Doodle::id);
            w.property("key", (std::string(*)(Doodle)) cdb::key);
        }
    }

    YQ_INVOKE(reg_doodle_meta();)
    template class Notifier<const Doodle::Diff&>;
}
