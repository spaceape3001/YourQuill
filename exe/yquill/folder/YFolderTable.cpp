////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "YFolderTable.hpp"
#include <dbgui/folder/FolderTableModel.hpp>
#include <dbgui/folder/FolderTableView.hpp>
#include <gui/validator/KeyValidator.hpp>
#include <util/Utilities.hpp>

#include <QLineEdit>
#include <QPushButton>
#include <QTimer>
#include <QToolBar>
#include <QVBoxLayout>



YFolderTable::YFolderTable(FolderProvider stp, QWidget*parent) : SubWin(parent), m_model{}, m_view{}, m_addKey{}, m_addBtn{}, m_addEnable{}
{
    if(!stp)
        stp     = provider::all_folders();
        
    m_model     = new FolderTableModel(stp, this);
    
    m_model -> setReadOnly(false);
    m_view      = new FolderTableView(m_model);
    
    QToolBar*   tb  = new QToolBar;
    m_addKey        = new QLineEdit;
    m_addKey -> setValidator(new KeyValidator(m_addKey));
    m_addBtn        = new QPushButton("+");
    setAddEnable(false);
    
    connect(m_addKey, &QLineEdit::returnPressed, this, &YFolderTable::cmdAdd);
    connect(m_addBtn, &QPushButton::clicked, this, &YFolderTable::cmdAdd);
    
    connect(m_view, &QTableView::doubleClicked, this, &YFolderTable::doubleClicked);
    
    tb->addWidget(m_addKey);
    tb->addWidget(m_addBtn);
    
    QVBoxLayout*    lay = new QVBoxLayout(this);
    lay -> addWidget(m_view, 1);
    lay -> addWidget(tb, 0);
    
    lay -> setContentsMargins(0,0,0,0);
    
    setWindowTitle("Folders");
}

YFolderTable::~YFolderTable()
{
}

void    YFolderTable::check() 
{
    m_model -> check();
}

void      YFolderTable::cmdAdd()
{
    if(m_addEnable){
        //QString txt = m_addKey -> text().simplified();
        //if(txt.isEmpty())
            //return ;
        //Folder t   = cdb::make_tag(txt);
        //if(t)
            //m_model -> append(t);
    }
    
    m_addKey -> clear();
}

void      YFolderTable::doubleClicked(const QModelIndex&idx)
{
    if(is_in({"Key", "ID"}, m_model->columnKey(idx))){
        Folder t   = m_model -> get(idx.row());
        if(t)
            emit editReq(t.id);
    }
}

void            YFolderTable::refresh()
{
    m_model -> refresh();
}

void            YFolderTable::setAddEnable(bool f)
{
    m_addEnable = f;
    m_addKey -> setEnabled(f);
    m_addBtn -> setEnabled(f);
}


void   YFolderTable::setReadOnly(bool f)
{
    m_model -> setReadOnly(f);
}


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#include "moc_YFolderTable.cpp"
