////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "IdList.hpp"
#include "IdListView.hpp"
#include <QVBoxLayout>

namespace yq::mithril {
    IdList::IdList(IdModel* mdl, QWidget* parent) : QWidget(parent)
    {
        m_model = mdl;
        m_model->setParent(this);
        m_view  = new IdListView(m_model);
        
        QVBoxLayout*    lay = new QVBoxLayout;
        lay->setContentsMargins(0,0,0,0);
        lay->addWidget(m_view);
        setLayout(lay);
    }
    
    IdList::~IdList()
    {
    }

    void    IdList::refresh()
    {
        m_model -> reload();
    }
    
    void    IdList::update()
    {
        m_model -> update();
    }
}

