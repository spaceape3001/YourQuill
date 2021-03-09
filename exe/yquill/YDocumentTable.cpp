////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "YDocumentTable.hpp"
#include <dbgui/filesys/DocumentTableModel.hpp>
#include <dbgui/filesys/DocumentTableView.hpp>
#include <gui/validator/KeyValidator.hpp>
#include <util/Utilities.hpp>

#include <QLineEdit>
#include <QPushButton>
#include <QTimer>
#include <QToolBar>
#include <QVBoxLayout>



YDocumentTable::YDocumentTable(DocumentProvider stp, QWidget*parent) : SubWin(parent), m_model{}, m_view{}, m_addKey{}, m_addBtn{}, m_addEnable{}
{
    if(!stp)
        stp     = provider::all_documents();
        
    m_model     = new DocumentTableModel(stp, this);
    
    m_model -> setReadOnly(false);
    m_view      = new DocumentTableView(m_model);
    
    QToolBar*   tb  = new QToolBar;
    m_addKey        = new QLineEdit;
    m_addKey -> setValidator(new KeyValidator(m_addKey));
    m_addBtn        = new QPushButton("+");
    setAddEnable(false);
    
    connect(m_addKey, &QLineEdit::returnPressed, this, &YDocumentTable::cmdAdd);
    connect(m_addBtn, &QPushButton::clicked, this, &YDocumentTable::cmdAdd);
    
    connect(m_view, &QTableView::doubleClicked, this, &YDocumentTable::doubleClicked);
    
    tb->addWidget(m_addKey);
    tb->addWidget(m_addBtn);
    
    QVBoxLayout*    lay = new QVBoxLayout(this);
    lay -> addWidget(m_view, 1);
    lay -> addWidget(tb, 0);
    
    lay -> setContentsMargins(0,0,0,0);
    
    setWindowTitle("Documents");
}

YDocumentTable::~YDocumentTable()
{
}

void    YDocumentTable::check() 
{
    m_model -> check();
}

void      YDocumentTable::cmdAdd()
{
    if(m_addEnable){
        //QString txt = m_addKey -> text().simplified();
        //if(txt.isEmpty())
            //return ;
        //Document t   = cdb::make_tag(txt);
        //if(t)
            //m_model -> append(t);
    }
    
    m_addKey -> clear();
}

void      YDocumentTable::doubleClicked(const QModelIndex&idx)
{
    if(is_in({"Key", "ID"}, m_model->columnKey(idx))){
        Document t   = m_model -> get(idx.row());
        if(t)
            emit editReq(t.id);
    }
}

void            YDocumentTable::refresh()
{
    m_model -> refresh();
}

void            YDocumentTable::setAddEnable(bool f)
{
    m_addEnable = f;
    m_addKey -> setEnabled(f);
    m_addBtn -> setEnabled(f);
}


void   YDocumentTable::setReadOnly(bool f)
{
    m_model -> setReadOnly(f);
}


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#include "moc_YDocumentTable.cpp"
