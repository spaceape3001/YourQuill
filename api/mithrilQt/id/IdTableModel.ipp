////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "IdTableModel.hpp"

namespace yq::mithril {
    IdTableModel::IdTableModel(Id i, IdProvider::UPtr p, QObject*parent) :
        IdModel(Type::Table, i, std::move(p), parent)
    {
    }
        
    IdTableModel::~IdTableModel()
    {
    }

    void    IdTableModel::setColumns(std::vector<IdColumn>&& cols)
    {
        m_columns       = std::move(cols);
    }
    
    void    IdTableModel::addColumn(IdColumn&& col)
    {
        m_columns.push_back(std::move(col));
    }
    
    void    IdTableModel::addColumn(size_t before, IdColumn&& col)
    {
        if(before < m_columns.size()){
            m_columns.insert(m_columns.begin()+before, std::move(col));
        } else {
            m_columns.push_back(std::move(col));
        }
    }
}

