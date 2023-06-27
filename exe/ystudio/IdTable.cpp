////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "IdTable.hpp"
#include <mithrilQt/id/IdTableView.hpp>
#include <QVBoxLayout>

IdTable::IdTable(IdModel* mdl, QWidget* parent) : Window(parent)
{
    m_model = mdl;
    m_model->setParent(this);
    m_view  = new IdTableView(m_model);
    
    connect(m_view, &IdTableView::popupRequested, [this](Id i){ 
        popupRequested(i);
    });
    
    connect(m_view, &IdTableView::doubleClicked, this, &IdTable::doubleClicked);

    QVBoxLayout*    lay = new QVBoxLayout;
    lay->setContentsMargins(0,0,0,0);
    lay->addWidget(m_view);
    setLayout(lay);
}

IdTable::~IdTable()
{
}

void    IdTable::doubleClicked(const QModelIndex&idx)
{
    Id i    = m_model->id(idx);
    if(i)
        emit openRequested(i);
}


void    IdTable::refresh()
{
    m_model -> reload();
}

void    IdTable::update()
{
    m_model -> update();
}

void    IdTable::finalize()
{
    m_view -> finalize();
}

#include "moc_IdTable.cpp"
