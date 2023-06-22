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
    //std::optional<IdColumn>     BookModel::resolve(Column col, ColOpts opts)
    //{
        //switch(col){
        //case Column::Id:
            //return column::book_id(opts);
        //case Column::Key:
            //return column::book_key(opts);
        //case Column::Title:
            //return column::book_title(opts);
        //default:
            //return {};
        //}
    //}

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
    
    //void    BookModel::addColumn(Column col, ColOpts opts)
    //{
        //auto    cc  = resolve(col, opts);
        //if(cc)
            //addColumn(std::move(*cc));
    //}
    
    //void    BookModel::addColumns(std::span<const Column> columns)
    //{
        //for(Column c : columns)
            //addColumn(c);
    //}

    //void    BookModel::setColumn(Column col, ColOpts opts)
    //{
        //auto    cc = resolve(col, opts);
        //if(cc)
            //setColumn(std::move(*cc));
    //}
}
