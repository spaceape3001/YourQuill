////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "IdModel.hpp"
#include "IdListView.hpp"
#include <QContextMenuEvent>

namespace yq::mithril {
    IdListView::IdListView(IdModel*m, QWidget*parent) : QListView(parent), m_model(m)
    {
        assert(m);
        setModel(m);
    }
    
    IdListView::~IdListView()
    {
    }

    void IdListView::contextMenuEvent(QContextMenuEvent*evt) 
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

