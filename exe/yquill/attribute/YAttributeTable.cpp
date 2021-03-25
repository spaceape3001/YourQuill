////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "YAttributeTable.hpp"
#include <dbgui/attribute/AttributeTableModel.hpp>
#include <dbgui/attribute/AttributeTableView.hpp>
#include <gui/validator/KeyValidator.hpp>
#include <util/Utilities.hpp>

#include <QLineEdit>
#include <QPushButton>
#include <QTimer>
#include <QToolBar>
#include <QVBoxLayout>



YAttributeTable::YAttributeTable(AttributeProvider stp, QWidget*parent) : SubWin(parent), m_model{}, m_view{}, m_addKey{}, m_addBtn{}, m_addEnable{}
{
    if(!stp)
        stp     = provider::all_attributes();
        
    m_model     = new AttributeTableModel(stp, this);
    
    m_model -> setReadOnly(false);
    m_view      = new AttributeTableView(m_model);
    
    QToolBar*   tb  = new QToolBar;
    m_addKey        = new QLineEdit;
    m_addKey -> setValidator(new KeyValidator(m_addKey));
    m_addBtn        = new QPushButton("+");
    setAddEnable(false);
    
    connect(m_addKey, &QLineEdit::returnPressed, this, &YAttributeTable::cmdAdd);
    connect(m_addBtn, &QPushButton::clicked, this, &YAttributeTable::cmdAdd);
    
    connect(m_view, &QTableView::doubleClicked, this, &YAttributeTable::doubleClicked);
    
    tb->addWidget(m_addKey);
    tb->addWidget(m_addBtn);
    
    QVBoxLayout*    lay = new QVBoxLayout(this);
    lay -> addWidget(m_view, 1);
    lay -> addWidget(tb, 0);
    
    lay -> setContentsMargins(0,0,0,0);
    
    setWindowTitle("Attributes");
}

YAttributeTable::~YAttributeTable()
{
}

void    YAttributeTable::check() 
{
    m_model -> check();
}

void      YAttributeTable::cmdAdd()
{
    if(m_addEnable){
        //QString txt = m_addKey -> text().simplified();
        //if(txt.isEmpty())
            //return ;
        //Attribute t   = cdb::make_tag(txt);
        //if(t)
            //m_model -> append(t);
    }
    
    m_addKey -> clear();
}

void      YAttributeTable::doubleClicked(const QModelIndex&idx)
{
    if(is_in({"Key", "ID"}, m_model->columnKey(idx))){
        Attribute t   = m_model -> get(idx.row());
        if(t)
            emit editReq(t.id);
    }
}

void            YAttributeTable::refresh()
{
    m_model -> refresh();
}

void            YAttributeTable::setAddEnable(bool f)
{
    m_addEnable = f;
    m_addKey -> setEnabled(f);
    m_addBtn -> setEnabled(f);
}


void   YAttributeTable::setReadOnly(bool f)
{
    m_model -> setReadOnly(f);
}


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#include "moc_YAttributeTable.cpp"
