////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "PlaceList.hpp"
#include "PlaceModel.hpp"

namespace yq::mithril {
    PlaceList::PlaceList(all_t, QWidget*parent) : PlaceList(ALL, defColumn, parent)
    {
    }
    
    PlaceList::PlaceList(all_t, Column col, QWidget*parent) : 
        IdListT<Place>(new PlaceModel(IdModel::Type::List, ALL), parent)
    {
        model()->setColumn(col);
        model()->reload();
        setWindowTitle("Places");
    }

    PlaceList::~PlaceList()
    {
    }
    
    PlaceModel*          PlaceList::model()
    {
        return static_cast<PlaceModel*>(m_model);
    }
    
    const PlaceModel*    PlaceList::model() const
    {
        return static_cast<const PlaceModel*>(m_model);
    }
}

