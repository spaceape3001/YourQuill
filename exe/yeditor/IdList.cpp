////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "IdList.hpp"
#include <mithrilQt/id/IdListView.hpp>
#include <QVBoxLayout>

IdList::IdList(IdModel* mdl, QWidget* parent) : Window(parent)
{
    m_model = mdl;
    m_model->setParent(this);
    m_view  = new IdListView(m_model);
    
    QVBoxLayout*    lay = new QVBoxLayout;
    lay->setContentsMargins(0,0,0,0);
    lay->addWidget(m_view);
    setLayout(lay);

    connect(m_view, &IdListView::popupRequested, [this](Id i){ 
        popupRequested(i);
    });

    connect(m_view, &IdListView::doubleClicked, this, &IdList::doubleClicked);
}

IdList::~IdList()
{
}

void    IdList::doubleClicked(const QModelIndex&idx)
{
    Id i    = m_model->id(idx);
    if(i)
        emit openRequested(i);
}

void    IdList::refresh()
{
    m_model -> reload();
}

void    IdList::update()
{
    m_model -> update();
}

#include "moc_IdList.cpp"
