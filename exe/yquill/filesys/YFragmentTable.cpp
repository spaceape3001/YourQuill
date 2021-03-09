////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "YFragmentTable.hpp"
#include <dbgui/filesys/FragmentTableModel.hpp>
#include <dbgui/filesys/FragmentTableView.hpp>
#include <gui/validator/KeyValidator.hpp>
#include <util/Utilities.hpp>

#include <QLineEdit>
#include <QPushButton>
#include <QTimer>
#include <QToolBar>
#include <QVBoxLayout>



YFragmentTable::YFragmentTable(FragmentProvider stp, QWidget*parent) : SubWin(parent), m_model{}, m_view{}, m_addKey{}, m_addBtn{}, m_addEnable{}
{
    if(!stp)
        stp     = provider::all_fragments();
        
    m_model     = new FragmentTableModel(stp, this);
    
    m_model -> setReadOnly(false);
    m_view      = new FragmentTableView(m_model);
    
    QToolBar*   tb  = new QToolBar;
    m_addKey        = new QLineEdit;
    m_addKey -> setValidator(new KeyValidator(m_addKey));
    m_addBtn        = new QPushButton("+");
    setAddEnable(false);
    
    connect(m_addKey, &QLineEdit::returnPressed, this, &YFragmentTable::cmdAdd);
    connect(m_addBtn, &QPushButton::clicked, this, &YFragmentTable::cmdAdd);
    
    connect(m_view, &QTableView::doubleClicked, this, &YFragmentTable::doubleClicked);
    
    tb->addWidget(m_addKey);
    tb->addWidget(m_addBtn);
    
    QVBoxLayout*    lay = new QVBoxLayout(this);
    lay -> addWidget(m_view, 1);
    lay -> addWidget(tb, 0);
    
    lay -> setContentsMargins(0,0,0,0);
    
    setWindowTitle("Fragments");
}

YFragmentTable::~YFragmentTable()
{
}

void    YFragmentTable::check() 
{
    m_model -> check();
}

void      YFragmentTable::cmdAdd()
{
    if(m_addEnable){
        //QString txt = m_addKey -> text().simplified();
        //if(txt.isEmpty())
            //return ;
        //Fragment t   = cdb::make_tag(txt);
        //if(t)
            //m_model -> append(t);
    }
    
    m_addKey -> clear();
}

void      YFragmentTable::doubleClicked(const QModelIndex&idx)
{
    if(is_in({"Key", "ID"}, m_model->columnKey(idx))){
        Fragment t   = m_model -> get(idx.row());
        if(t)
            emit editReq(t.id);
    }
}

void            YFragmentTable::refresh()
{
    m_model -> refresh();
}

void            YFragmentTable::setAddEnable(bool f)
{
    m_addEnable = f;
    m_addKey -> setEnabled(f);
    m_addBtn -> setEnabled(f);
}


void   YFragmentTable::setReadOnly(bool f)
{
    m_model -> setReadOnly(f);
}


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#include "moc_YFragmentTable.cpp"
