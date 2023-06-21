////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RootList.hpp"
#include "RootModel.hpp"

namespace yq::mithril {
    RootList::RootList(all_t, QWidget*parent) : RootList(ALL, defColumn, parent)
    {
    }
    
    RootList::RootList(all_t, Column col, QWidget*parent) : 
        IdListT<Root>(new RootModel(IdModel::Type::List, ALL), parent)
    {
        model()->setColumn(col);
        model()->reload();
        setWindowTitle("Roots");
    }

    RootList::~RootList()
    {
    }
    
    RootModel*          RootList::model()
    {
        return static_cast<RootModel*>(m_model);
    }
    
    const RootModel*    RootList::model() const
    {
        return static_cast<const RootModel*>(m_model);
    }
}

