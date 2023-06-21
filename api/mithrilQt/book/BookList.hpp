////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdList.hpp>

namespace yq::mithril {
    class BookModel;
    class BookList : public IdListT<Book> {
        Q_OBJECT
    public:
        static constexpr const Column   defColumn   = Column::Key;
    
        BookList(all_t, QWidget*parent=nullptr);
        BookList(all_t, Column col, QWidget*parent=nullptr);
        ~BookList();
        
        BookModel*          model();
        const BookModel*    model() const;
    };
}
