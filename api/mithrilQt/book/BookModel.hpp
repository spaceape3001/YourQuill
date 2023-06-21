////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdModel.hpp>
#include <mithril/book/Book.hpp>
#include <mithrilQt/preamble.hpp>

namespace yq::mithril {
    class BookModel : public IdModelT<Book> {
        Q_OBJECT
    public:
    
        static std::optional<IdColumn>     resolve(Column, ColOpts opts=ColOpts());

        BookModel(Type t,QObject* parent=nullptr) : BookModel(t, ALL, parent) {}
        BookModel(Type t, all_t, QObject* parent=nullptr);
        ~BookModel();
        
        using IdModelT<Book>::addColumn;
        void    addColumn(Column, ColOpts opts=ColOpts());
        void    addColumns(std::span<const Column> columns);
        
        using IdModelT<Book>::setColumn;
        void    setColumn(Column, ColOpts opts=ColOpts());
    private:
        BookModel(Type t, Book, IdProvider&&, QObject*parent);
    };
}
