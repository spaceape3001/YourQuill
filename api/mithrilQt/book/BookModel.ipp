////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "BookModel.hpp"
#include "BookColumn.hpp"
#include <mithril/book/BookProvider.hpp>

namespace yq::mithril {
    std::span<const Column>  BookModel::defColumns() 
    {
        static Column   s_data[] = { Column::Key, Column::Title };
        return std::span<const Column>(std::begin(s_data), std::end(s_data));
    }

    BookModel::BookModel(Type t, all_t, std::span<const Column> cols, QObject* parent) : 
        BookModel(t, Book(), provider::all_books(), cols, parent)
    {
    }

    BookModel::BookModel(Type t, Book rt, IdProvider&& prov, std::span<const Column> cols, QObject*parent) : 
        IdModelT<Book>(t, rt, std::move(prov), parent)
    {
        for(Column c : cols)
            addColumn(c);
        reload();
    }
    
    BookModel::~BookModel()
    {
    }
    
    void    BookModel::addColumn(Column col)
    {
        switch(col){
        case Column::Id:
            addColumn(column::book_id());
            break;
        case Column::Key:
            addColumn(column::book_key());
            break;
        case Column::Title:
            addColumn(column::book_title());
            break;
        default:
            break;
        }
    }
    
}
