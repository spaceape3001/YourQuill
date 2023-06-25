////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "IdTable.hpp"
#include <mithrilQt/id/IdTableView.hpp>
#include <QVBoxLayout>

IdTable::IdTable(IdModel* mdl, QWidget* parent) : QWidget(parent)
{
    m_model = mdl;
    m_model->setParent(this);
    m_view  = new IdTableView(m_model);
    
    QVBoxLayout*    lay = new QVBoxLayout;
    lay->setContentsMargins(0,0,0,0);
    lay->addWidget(m_view);
    setLayout(lay);
}

IdTable::~IdTable()
{
}

void    IdTable::refresh()
{
    m_model -> reload();
}

void    IdTable::update()
{
    m_model -> update();
}

#include "moc_IdTable.cpp"
