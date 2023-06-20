////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdModel.hpp>
#include <mithril/book/Book.hpp>
#include <mithrilQt/column.hpp>

namespace yq::mithril {
    class BookModel : public IdModelT<Book> {
        Q_OBJECT
    public:
    
        static std::span<const Column>  defColumns();
    
        BookModel(Type t) : BookModel(t, ALL) {}
        BookModel(Type t, all_t, std::span<const Column> cols=defColumns(), QObject* parent=nullptr);
        ~BookModel();
        
        using IdModelT<Book>::addColumn;
        void    addColumn(Column);
        
    private:
        BookModel(Type t, Book, IdProvider&&, std::span<const Column> cols, QObject*parent);
    };
}
