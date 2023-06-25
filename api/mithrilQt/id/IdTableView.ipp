////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "IdModel.hpp"
#include "IdTableView.hpp"
#include <QContextMenuEvent>

namespace yq::mithril {
    IdTableView::IdTableView(IdModel*m, QWidget*parent) : QTableView(parent), m_model(m)
    {
        assert(m);
        setModel(m);
    }
    
    IdTableView::~IdTableView()
    {
    }

    void IdTableView::contextMenuEvent(QContextMenuEvent*evt) 
    {
        evt -> accept();
        QModelIndex idx = indexAt(evt->pos());
        if(!idx.isValid())
            return ;
        Id      i   = m_model -> id(idx);
        if(!i)
            return ;
        emit popupRequested(i);
    }
}

