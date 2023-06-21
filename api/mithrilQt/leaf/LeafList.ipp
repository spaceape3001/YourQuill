////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "LeafList.hpp"
#include "LeafModel.hpp"

namespace yq::mithril {
    LeafList::LeafList(all_t, QWidget*parent) : LeafList(ALL, defColumn, parent)
    {
    }
    
    LeafList::LeafList(all_t, Column col, QWidget*parent) : 
        IdListT<Leaf>(new LeafModel(IdModel::Type::List, ALL), parent)
    {
        model()->setColumn(col);
        model()->reload();
        setWindowTitle("Leafs");
    }

    LeafList::~LeafList()
    {
    }
    
    LeafModel*          LeafList::model()
    {
        return static_cast<LeafModel*>(m_model);
    }
    
    const LeafModel*    LeafList::model() const
    {
        return static_cast<const LeafModel*>(m_model);
    }
}

