////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "BookCDB.hpp"
#include <yq/meta/TypeMetaWriter.hpp>
#include <yq/meta/Init.hpp>
#include <mithril/class/Class.hpp>
#include <mithril/image/Image.hpp>

YQ_TYPE_IMPLEMENT(yq::mithril::Book)
YQ_TYPE_IMPLEMENT(yq::mithril::BookSet)
YQ_TYPE_IMPLEMENT(yq::mithril::BookVector)

namespace yq::mithril {
    void reg_book_meta()
    {
        {
            auto w  = writer<Book>();
            w.property("atom", (Atom(*)(Book)) cdb::atom);
            w.property("class", (Class(*)(Book)) cdb::book_class);
            w.property("icon", (Image(*)(Book)) cdb::icon);
            w.property("id", READ_ONLY, &Book::id);
            w.property("key", (std::string(*)(Book)) cdb::key);
            w.property("name", (std::string(*)(Book)) cdb::name);
            w.property("title", (std::string(*)(Book)) cdb::title);
        }
    }
    
    YQ_INVOKE(reg_book_meta();)
}

