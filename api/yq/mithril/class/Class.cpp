////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Class.hpp"
#include "ClassCDB.hpp"

#include <mithril/category/Category.hpp>
#include <mithril/document/Document.hpp>
#include <mithril/image/Image.hpp>

#include <yq/text/match.hpp>

#include <yq/meta/TypeMetaWriter.hpp>
#include <yq/meta/Init.hpp>
#include <mithril/notify/Notifier.hxx>

YQ_TYPE_IMPLEMENT(yq::mithril::Class)
YQ_TYPE_IMPLEMENT(yq::mithril::ClassSet)
YQ_TYPE_IMPLEMENT(yq::mithril::ClassVector)

namespace yq::mithril {
    bool Class::less_key(Class a, Class b)
    {
        return is_less_igCase(cdb::key(a), cdb::key(b));
    }

    bool Class::less_label(Class a, Class b)
    {
        return is_less_igCase(cdb::label(a), cdb::label(b));
    }

    bool Class::less_name(Class a, Class b)
    {
        return is_less_igCase(cdb::name(a), cdb::name(b));
    }

    void reg_class_meta()
    {
        {
            auto w  = writer<Class>();
            w.property("binding", (std::string(*)(Class)) cdb::binding);
            w.property("brief", (std::string(*)(Class)) cdb::brief);
            w.property("category", (Category(*)(Class)) cdb::category);
            w.property("document", (Document(*)(Class)) cdb::document);
            w.property("icon", (Image(*)(Class)) cdb::icon);
            w.property("id", READ_ONLY, &Class::id);
            w.property("key", (std::string(*)(Class)) cdb::key);
            w.property("label", (std::string(*)(Class)) cdb::label);
            w.property("name", (std::string(*)(Class)) cdb::name);
            w.property("plural", (std::string(*)(Class)) cdb::plural);
            w.property("url", (std::string(*)(Class)) cdb::url);
        }
    }
    YQ_INVOKE(reg_class_meta();)

    template class Notifier<const Class::Diff&>;
}
