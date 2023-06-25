////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "IdModel.hpp"
#include "IdTreeView.hpp"
#include <QContextMenuEvent>

namespace yq::mithril {
    IdTreeView::IdTreeView(IdModel*m, QWidget*parent) : QTreeView(parent), m_model(m)
    {
        assert(m);
        setModel(m);
    }
    
    IdTreeView::~IdTreeView()
    {
    }

    void IdTreeView::contextMenuEvent(QContextMenuEvent*evt)
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

