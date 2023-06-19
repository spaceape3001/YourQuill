////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "IdTable.hpp"
#include <QVBoxLayout>

namespace yq::mithril {
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    IdTable::IdTable(Id rt, IdProvider&& prov, QWidget* parent) : QWidget(parent)
    {
        m_model = new Model(rt, std::move(prov), this);
        m_view  = new View(m_model);
        
        QVBoxLayout*    lay = new QVBoxLayout;
        lay->setContentsMargins(0,0,0,0);
        lay->addWidget(m_view);
        setLayout(lay);
    }
    
    IdTable::IdTable(IdProvider&& prov, QWidget*parent) : IdTable(Id(), std::move(prov), parent)
    {
    }
    
    IdTable::~IdTable()
    {
    }

    void    IdTable::addColumn(IdColumn&&col)
    {
        m_model -> addColumn(std::move(col));
    }
    
    void    IdTable::addColumn(size_t before, IdColumn&& col)
    {
        m_model -> addColumn(before, std::move(col));
    }

    void    IdTable::refresh()
    {
        m_model -> reload();
    }
    
    void    IdTable::update()
    {
        m_model -> update();
    }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    IdTable::Model::Model(Id i, IdProvider&& p, QObject*parent) :
        IdModel(Type::Table, i, std::move(p), parent)
    {
    }
        
    IdTable::Model::~Model()
    {
    }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    IdTable::View::View(Model*m, QWidget*parent) : QTableView(parent), m_model(m)
    {
        assert(m);
        setModel(m);
        //for(const IdColumn& col : m_model->columns()){
        //}
    }
    
    IdTable::View::~View()
    {
    }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
}

