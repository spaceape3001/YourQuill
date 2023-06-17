////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "IdListModel.hpp"

namespace yq::mithril {
    
    
    IdListModel::IdListModel(Id i, IdProvider::UPtr&& provider, QObject*parent) :
        IdModel(Type::List, i, std::move(provider), parent)
    {
    }
    
    IdListModel::~IdListModel()
    {
    }

    void    IdListModel::setColumn(IdColumn&&col)
    {
        if(m_columns.empty()){
            m_columns.push_back(col);
        } else {
            m_columns[0]        = std::move(col);
        }
    }
}
