////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "IdItemModel.hpp"

namespace yq::mithril {

    IdItemModel::IdItemModel(Id i, IdProvider::UPtr upp, TreeDetector&&tdetect, ProviderGenerator&&pgen, QObject*parent) : 
        IdModel(Type::Tree, i, std::move(upp), parent)
    {
        m_treeDetect    = std::move(tdetect);
        m_treeGenerator = std::move(pgen);
    }
    
    IdItemModel::~IdItemModel()
    {
    }
    
    void    IdItemModel::setColumns(std::vector<IdColumn>&& cols)
    {
        m_columns       = std::move(cols);
    }
    
    void    IdItemModel::addColumn(IdColumn&& col)
    {
        m_columns.push_back(std::move(col));
    }
    
    void    IdItemModel::addColumn(size_t before, IdColumn&& col)
    {
        if(before < m_columns.size()){
            m_columns.insert(m_columns.begin()+before, std::move(col));
        } else {
            m_columns.push_back(std::move(col));
        }
    }
}

