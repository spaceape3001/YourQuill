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
    std::span<const Column>  DocumentModel::defColumns() 
    {
        static Column   s_data[] = { Column::Key, Column::Name };
        return std::span<const Column>(std::begin(s_data), std::end(s_data));
    }

    DocumentModel::DocumentModel(Type t, all_t, std::span<const Column> cols, QObject* parent) : 
        DocumentModel(t, Document(), provider::all_documents(), cols, parent)
    {
    }

    DocumentModel::DocumentModel(Type t, Document rt, IdProvider&& prov, std::span<const Column> cols, QObject*parent) : 
        IdModelT<Document>(t, rt, std::move(prov), parent)
    {
        for(Column c : cols)
            addColumn(c);
        reload();
    }
    
    DocumentModel::~DocumentModel()
    {
    }
    
    void    DocumentModel::addColumn(Column col)
    {
        switch(col){
        case Column::Id:
            addColumn(column::document_id());
            break;
        case Column::Key:
            addColumn(column::document_key());
            break;
        case Column::Name:
            addColumn(column::document_name());
            break;
        default:
            break;
        }
    }
    
}
