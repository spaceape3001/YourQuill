////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/meta/TypeInfoWriter.hpp>
#include "BookCDB.hpp"

namespace {
    void reg_book_meta()
    {
        using namespace yq;
        using namespace yq::mithril;
        
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
}
