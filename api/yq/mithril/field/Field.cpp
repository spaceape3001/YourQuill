////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Field.hpp"
#include "FieldCDB.hpp"

#include <yq/meta/TypeMetaWriter.hpp>
#include <yq/meta/Init.hpp>
#include <yq/text/match.hpp>
#include <yq/mithril/notify/Notifier.hxx>

YQ_TYPE_IMPLEMENT(yq::mithril::Field)
YQ_TYPE_IMPLEMENT(yq::mithril::FieldSet)
YQ_TYPE_IMPLEMENT(yq::mithril::FieldVector)

namespace yq::mithril {
    bool Field::less_key(Field a, Field b)
    {
        return is_less_igCase(cdb::key(a), cdb::key(b));
    }

    bool Field::less_label(Field a, Field b)
    {
        return is_less_igCase(cdb::label(a), cdb::label(b));
    }

    bool Field::less_name(Field a, Field b)
    {
        return is_less_igCase(cdb::name(a), cdb::name(b));
    }

    void reg_field_meta()
    {
        {
            auto w  = writer<Field>();
            w.property("id", READ_ONLY, &Field::id);
            w.property("key", (std::string(*)(Field)) cdb::key);
        }
    }

    YQ_INVOKE(reg_field_meta();)
    template class Notifier<const Field::Diff&>;
}
