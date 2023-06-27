////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "IdModel.hpp"
#include "IdTableView.hpp"
#include <QContextMenuEvent>
#include <gluon/delegate/Delegate.hpp>

using namespace yq::gluon;

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

    void IdTableView::finalize()
    {
        for(size_t n=0;n<m_model->columnCount();++n){
            Delegate*   del = m_model -> createDelegate(n);
            if(!del)
                continue;
            del->setParent(this);
            setItemDelegateForColumn(n, del);
        }
    }
}

