////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "book.hpp"

#include <basic/DelayInit.hpp>
#include <gluon/core/Utilities.hpp>
#include <mithril/book/BookCDB.hpp>
#include <mithril/class/ClassCDB.hpp>
#include <mithril/image/Image.hpp>
#include <mithrilQt/image.hpp>
#include <QIcon>

namespace yq::mithril {
    QIcon  qIcon(Book b)
    {
        static QIcon    qico(":/generic/book.svg");
        if(!b)
            return QIcon();
        Image   img = cdb::icon(b);
        if(img)
            return qIcon(img);
        return qico;
    }
}

namespace yq::mithril::column {
    IdColumn    book_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::book_id();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::book_icon();
        ret.fnCompare           = compareFN::book_id();
        ret.label               = "ID";
        return ret;
    }
    
    IdColumn    book_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::book_key();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::book_icon();
        ret.fnCompare           = compareFN::book_key();
        ret.label               = "Key";
        return ret;
    }
    
    IdColumn    book_title(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::book_title();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::book_icon();
        ret.fnCompare           = compareFN::book_title();
        ret.label               = "Title";
        return ret;
    }

    void    reg_book_columns()
    {
        IdColumn::declare<Book>(Column::Id,    book_id);
        IdColumn::declare<Book>(Column::Key,   book_key);
        IdColumn::declare<Book>(Column::Title, book_title);

        IdColumn::set_defaultList<Book>({ Column::Key, ColOpt::Icon });
        IdColumn::set_defaultTable<Book>({{ Column::Key, ColOpt::Icon }, Column::Title });
    }
    
    YQ_INVOKE(reg_book_columns();)
}

namespace yq::mithril::compareFN {
    IdColumn::CompareFN book_id()
    {
        return [](Id a, Id b) -> Compare {
            return compare(a.as<Book>().id, b.as<Book>().id);
        };
    }
    
    IdColumn::CompareFN book_key()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::key(a.as<Book>()), cdb::key(b.as<Book>()));
        };
    }
    
    IdColumn::CompareFN book_title()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::title(a.as<Book>()), cdb::title(b.as<Book>()));
        };
    }
}

namespace yq::mithril::decorationFN {
    IdColumn::VariantFN  book_icon()
    {
        return [](Id i) -> QVariant {
            return qIcon(i.as<Book>());
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


