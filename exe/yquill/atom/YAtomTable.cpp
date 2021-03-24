////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "YAtomTable.hpp"
#include <dbgui/atom/AtomTableModel.hpp>
#include <dbgui/atom/AtomTableView.hpp>
#include <gui/validator/KeyValidator.hpp>
#include <util/Utilities.hpp>

#include <QLineEdit>
#include <QPushButton>
#include <QTimer>
#include <QToolBar>
#include <QVBoxLayout>



YAtomTable::YAtomTable(AtomProvider stp, QWidget*parent) : SubWin(parent), m_model{}, m_view{}, m_addKey{}, m_addBtn{}, m_addEnable{}
{
    if(!stp)
        stp     = provider::all_atoms();
        
    m_model     = new AtomTableModel(stp, this);
    
    m_model -> setReadOnly(false);
    m_view      = new AtomTableView(m_model);
    
    QToolBar*   tb  = new QToolBar;
    m_addKey        = new QLineEdit;
    m_addKey -> setValidator(new KeyValidator(m_addKey));
    m_addBtn        = new QPushButton("+");
    setAddEnable(false);
    
    connect(m_addKey, &QLineEdit::returnPressed, this, &YAtomTable::cmdAdd);
    connect(m_addBtn, &QPushButton::clicked, this, &YAtomTable::cmdAdd);
    
    connect(m_view, &QTableView::doubleClicked, this, &YAtomTable::doubleClicked);
    
    tb->addWidget(m_addKey);
    tb->addWidget(m_addBtn);
    
    QVBoxLayout*    lay = new QVBoxLayout(this);
    lay -> addWidget(m_view, 1);
    lay -> addWidget(tb, 0);
    
    lay -> setContentsMargins(0,0,0,0);
    
    setWindowTitle("Atoms");
}

YAtomTable::~YAtomTable()
{
}

void    YAtomTable::check() 
{
    m_model -> check();
}

void      YAtomTable::cmdAdd()
{
    if(m_addEnable){
        //QString txt = m_addKey -> text().simplified();
        //if(txt.isEmpty())
            //return ;
        //Atom t   = cdb::make_tag(txt);
        //if(t)
            //m_model -> append(t);
    }
    
    m_addKey -> clear();
}

void      YAtomTable::doubleClicked(const QModelIndex&idx)
{
    if(is_in({"Key", "ID"}, m_model->columnKey(idx))){
        Atom t   = m_model -> get(idx.row());
        if(t)
            emit editReq(t.id);
    }
}

void            YAtomTable::refresh()
{
    m_model -> refresh();
}

void            YAtomTable::setAddEnable(bool f)
{
    m_addEnable = f;
    m_addKey -> setEnabled(f);
    m_addBtn -> setEnabled(f);
}


void   YAtomTable::setReadOnly(bool f)
{
    m_model -> setReadOnly(f);
}


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#include "moc_YAtomTable.cpp"
