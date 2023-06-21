////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "IdModel.hpp"
#include "IdTreeView.hpp"

namespace yq::mithril {
    IdTreeView::IdTreeView(IdModel*m, QWidget*parent) : QTreeView(parent), m_model(m)
    {
        assert(m);
        setModel(m);
    }
    
    IdTreeView::~IdTreeView()
    {
    }

}

