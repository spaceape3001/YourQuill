////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "CategoryCDB.hpp"
#include <yq/meta/TypeMetaWriter.hpp>
#include <yq/meta/Init.hpp>

#include <yq/mithril/document/Document.hpp>
#include <yq/mithril/image/Image.hpp>
#include <yq/mithril/notify/Notifier.hxx>

YQ_TYPE_IMPLEMENT(yq::mithril::Category)
YQ_TYPE_IMPLEMENT(yq::mithril::CategorySet)
YQ_TYPE_IMPLEMENT(yq::mithril::CategoryVector)

namespace yq::mithril {
    void reg_category_meta()
    {
        {
            auto w  = writer<Category>();
            w.property("brief", (std::string(*)(Category)) cdb::brief);
            w.property("document", (Document(*)(Category)) cdb::document);
            w.property("icon", (Image(*)(Category)) cdb::icon);
            w.property("id", READ_ONLY, &Category::id);
            w.property("key", (std::string(*)(Category)) cdb::key);
            w.property("label", (std::string(*)(Category)) cdb::label);
            w.property("name", (std::string(*)(Category)) cdb::name);
        }
    }
    
    YQ_INVOKE(reg_category_meta();)
    template class Notifier<const Category::Diff&>;
}
