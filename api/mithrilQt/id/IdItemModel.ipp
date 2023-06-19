////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "IdItemModel.hpp"

namespace yq::mithril {

    IdItemModel::IdItemModel(Id i, IdProvider&& upp, TreeDetector&&tdetect, ProviderGenerator&&pgen, QObject*parent) : 
        IdModel(Type::Tree, i, std::move(upp), parent)
    {
        m_treeDetect    = std::move(tdetect);
        m_treeGenerator = std::move(pgen);
    }
    
    IdItemModel::~IdItemModel()
    {
    }
}

