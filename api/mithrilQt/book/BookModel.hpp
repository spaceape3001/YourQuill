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
    
        BookModel(Type t,QObject* parent=nullptr) : BookModel(t, ALL, parent) {}
        BookModel(Type t, all_t, QObject* parent=nullptr);
        ~BookModel();
        
        using IdModelT<Book>::addColumn;
        void    addColumn(Column);
        void    addColumns(std::span<const Column> columns);
        
    private:
        BookModel(Type t, Book, IdProvider&&, QObject*parent);
    };
}
