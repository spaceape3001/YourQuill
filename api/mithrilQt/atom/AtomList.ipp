////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AtomList.hpp"
#include "AtomModel.hpp"

namespace yq::mithril {
    AtomList::AtomList(all_t, QWidget*parent) : AtomList(ALL, defColumn, parent)
    {
    }
    
    AtomList::AtomList(all_t, Column col, QWidget*parent) : 
        IdListT<Atom>(new AtomModel(IdModel::Type::List, ALL), parent)
    {
        model()->setColumn(col);
        model()->reload();
        setWindowTitle("Atoms");
    }

    AtomList::~AtomList()
    {
    }
    
    AtomModel*          AtomList::model()
    {
        return static_cast<AtomModel*>(m_model);
    }
    
    const AtomModel*    AtomList::model() const
    {
        return static_cast<const AtomModel*>(m_model);
    }
}

