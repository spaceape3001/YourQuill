////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DocumentModel.hpp"
#include "DocumentColumn.hpp"
#include <mithril/document/DocumentProvider.hpp>

namespace yq::mithril {
    std::optional<IdColumn>     DocumentModel::resolve(Column col, ColOpts opts)
    {
        switch(col){
        case Column::Id:
            return column::document_id(opts);
        case Column::Key:
            return column::document_key(opts);
        case Column::Name:
            return column::document_name(opts);
        default:
            return {};
        }
    }

    DocumentModel::DocumentModel(Type t, all_t, QObject* parent) : 
        DocumentModel(t, Document(), provider::all_documents(), parent)
    {
    }

    DocumentModel::DocumentModel(Type t, Document rt, IdProvider&& prov, QObject*parent) : 
        IdModelT<Document>(t, rt, std::move(prov), parent)
    {
    }
    
    DocumentModel::~DocumentModel()
    {
    }
    
    void    DocumentModel::addColumn(Column col, ColOpts opts)
    {
        auto    cc  = resolve(col, opts);
        if(cc)
            addColumn(std::move(*cc));
    }
    
    void    DocumentModel::addColumns(std::span<const Column> columns)
    {
        for(Column c : columns)
            addColumn(c);
    }

    void    DocumentModel::setColumn(Column col, ColOpts opts)
    {
        auto    cc = resolve(col, opts);
        if(cc)
            setColumn(std::move(*cc));
    }
}
