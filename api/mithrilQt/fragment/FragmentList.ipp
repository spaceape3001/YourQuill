////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FragmentList.hpp"
#include "FragmentModel.hpp"

namespace yq::mithril {
    FragmentList::FragmentList(all_t, QWidget*parent) : FragmentList(ALL, defColumn, parent)
    {
    }
    
    FragmentList::FragmentList(all_t, Column col, QWidget*parent) : 
        IdListT<Fragment>(new FragmentModel(IdModel::Type::List, ALL), parent)
    {
        model()->setColumn(col);
        model()->reload();
        setWindowTitle("Fragments");
    }

    FragmentList::~FragmentList()
    {
    }
    
    FragmentModel*          FragmentList::model()
    {
        return static_cast<FragmentModel*>(m_model);
    }
    
    const FragmentModel*    FragmentList::model() const
    {
        return static_cast<const FragmentModel*>(m_model);
    }
}

