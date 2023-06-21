////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "IdModel.hpp"
#include "IdListView.hpp"

namespace yq::mithril {
    IdListView::IdListView(IdModel*m, QWidget*parent) : QListView(parent), m_model(m)
    {
        assert(m);
        setModel(m);
    }
    
    IdListView::~IdListView()
    {
    }

}

