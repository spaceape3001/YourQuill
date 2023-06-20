////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DocumentTable.hpp"
#include "DocumentModel.hpp"


namespace yq::mithril {

    DocumentTable::DocumentTable(all_t, QWidget*parent) : DocumentTable(ALL, DocumentModel::defColumns(), parent)
    {
    }
    
    DocumentTable::DocumentTable(all_t, std::initializer_list<Column> columns, QWidget*parent) : 
        DocumentTable(ALL, span(columns), parent)
    {
    }
    
    DocumentTable::DocumentTable(all_t, std::span<const Column> columns, QWidget*parent) : 
        IdTableT<Document>(new DocumentModel(IdModel::Type::Table, ALL, columns), parent)
    {
        setWindowTitle("Documents");
    }

    DocumentTable::~DocumentTable()
    {
    }
    
    
    DocumentModel*          DocumentTable::model()
    {
        return static_cast<DocumentModel*>(m_model);
    }
    
    const DocumentModel*    DocumentTable::model() const
    {
        return static_cast<const DocumentModel*>(m_model);
    }
}

