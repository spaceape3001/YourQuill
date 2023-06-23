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
#include <mithril/class/ClassCDB.hpp>
#include <mithril/image/Image.hpp>
#include <mithrilQt/image/ImageUtils.hpp>
#include <QIcon>

namespace yq::mithril::column {
    IdColumn    book_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::book_id();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::book_icon();
        ret.label               = "ID";
        return ret;
    }
    
    IdColumn    book_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::book_key();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::book_icon();
        ret.label               = "Key";
        return ret;
    }
    
    IdColumn    book_title(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::book_title();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::book_icon();
        ret.label               = "Title";
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

namespace yq::mithril::decorationFN {
    IdColumn::VariantFN  book_icon()
    {
        static QIcon    qico(":/generic/book.svg");
        return [](Id i) -> QVariant {
            Book   a   = i.as<Book>();
            if(!a)
                return QVariant();
            Image   img = cdb::icon(a);
            if(img)
                return qIcon(img);
            return qico;
        };
    }
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


