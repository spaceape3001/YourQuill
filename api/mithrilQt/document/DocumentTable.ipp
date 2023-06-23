////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DocumentTable.hpp"
#include "DocumentModel.hpp"


namespace yq::mithril {
    std::span<const ColumnSpec>  DocumentTable::defColumns() 
    {
        static ColumnSpec   s_data[] = { { Column::Key, ColOpt::Icon}, Column::Name };
        return std::span<const ColumnSpec>(std::begin(s_data), std::end(s_data));
    }


    DocumentTable::DocumentTable(all_t, QWidget*parent) : DocumentTable(ALL, defColumns(), parent)
    {
    }
    
    DocumentTable::DocumentTable(all_t, std::initializer_list<ColumnSpec> columns, QWidget*parent) : 
        DocumentTable(ALL, span(columns), parent)
    {
    }
    
    DocumentTable::DocumentTable(all_t, std::span<const ColumnSpec> columns, QWidget*parent) : 
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

