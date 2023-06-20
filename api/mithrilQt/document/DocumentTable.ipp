////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DocumentTable.hpp"
#include "DocumentModel.hpp"


namespace yq::mithril {
    std::span<const Column>  DocumentTable::defColumns() 
    {
        static Column   s_data[] = { Column::Key, Column::Name };
        return std::span<const Column>(std::begin(s_data), std::end(s_data));
    }


    DocumentTable::DocumentTable(all_t, QWidget*parent) : DocumentTable(ALL, defColumns(), parent)
    {
    }
    
    DocumentTable::DocumentTable(all_t, std::initializer_list<Column> columns, QWidget*parent) : 
        DocumentTable(ALL, span(columns), parent)
    {
    }
    
    DocumentTable::DocumentTable(all_t, std::span<const Column> columns, QWidget*parent) : 
        IdTableT<Document>(new DocumentModel(IdModel::Type::Table, ALL), parent)
    {
        model()->addColumns(columns);
        model()->reload();
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

