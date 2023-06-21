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
    BookModel::BookModel(Type t, all_t, QObject* parent) : 
        BookModel(t, Book(), provider::all_books(), parent)
    {
    }

    BookModel::BookModel(Type t, Book rt, IdProvider&& prov, QObject*parent) : 
        IdModelT<Book>(t, rt, std::move(prov), parent)
    {
    }
    
    BookModel::~BookModel()
    {
    }
    
    void    BookModel::addColumn(Column col, ColOpts opts)
    {
        switch(col){
        case Column::Id:
            addColumn(column::book_id(opts));
            break;
        case Column::Key:
            addColumn(column::book_key(opts));
            break;
        case Column::Title:
            addColumn(column::book_title(opts));
            break;
        default:
            break;
        }
    }
    
    void    BookModel::addColumns(std::span<const Column> columns)
    {
        for(Column c : columns)
            addColumn(c);
    }
}
