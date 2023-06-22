////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "BookColumn.hpp"

#include <basic/DelayInit.hpp>
#include <gluon/core/Utilities.hpp>
#include <mithril/book/BookCDB.hpp>

namespace yq::mithril::column {
    IdColumn    book_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::book_id();
        ret.label       = "ID";
        return ret;
    }
    
    IdColumn    book_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::book_key();
        ret.label       = "Key";
        return ret;
    }
    
    IdColumn    book_title(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::book_title();
        ret.label       = "Title";
        return ret;
    }

    void    reg_book_columns()
    {
        IdColumn::declare<Book>(Column::Id,    book_id);
        IdColumn::declare<Book>(Column::Key,   book_key);
        IdColumn::declare<Book>(Column::Title, book_title);
    }
    
    YQ_INVOKE(reg_book_columns();)
}

namespace yq::mithril::displayFN {
    IdColumn::VariantFN    book_id()
    {
        return [](Id i) -> QVariant {
            Book    a   = i.as<Book>();
            if(!a)
                return QVariant();
            return (quint64) a.id;
        };
    }
    
    IdColumn::VariantFN    book_key()
    {
        return [](Id i) -> QVariant {
            Book    a   = i.as<Book>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::key(a));
        };
    }
    
    IdColumn::VariantFN    book_title()
    {
        return [](Id i) -> QVariant {
            Book    a   = i.as<Book>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::title(a));
        };
    }
}


