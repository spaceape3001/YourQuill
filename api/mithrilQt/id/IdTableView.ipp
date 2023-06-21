////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "IdModel.hpp"
#include "IdTableView.hpp"

namespace yq::mithril {
    IdTableView::IdTableView(IdModel*m, QWidget*parent) : QTableView(parent), m_model(m)
    {
        assert(m);
        setModel(m);
    }
    
    IdTableView::~IdTableView()
    {
    }

}

