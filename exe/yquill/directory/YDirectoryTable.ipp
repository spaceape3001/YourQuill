////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once


YDirectoryTable::YDirectoryTable(DirectoryProvider stp, QWidget*parent) : SubWin(parent), m_model{}, m_view{}, m_addKey{}, m_addBtn{}, m_addEnable{}
{
    if(!stp)
        stp     = provider::all_directories();
        
    m_model     = new DirectoryTableModel(stp, this);
    
    m_model -> setReadOnly(false);
    m_view      = new DirectoryTableView(m_model);
    
    QToolBar*   tb  = new QToolBar;
    m_addKey        = new QLineEdit;
    m_addKey -> setValidator(new KeyValidator(m_addKey));
    m_addBtn        = new QPushButton("+");
    setAddEnable(false);
    
    connect(m_addKey, &QLineEdit::returnPressed, this, &YDirectoryTable::cmdAdd);
    connect(m_addBtn, &QPushButton::clicked, this, &YDirectoryTable::cmdAdd);
    
    connect(m_view, &QTableView::doubleClicked, this, &YDirectoryTable::doubleClicked);
    
    tb->addWidget(m_addKey);
    tb->addWidget(m_addBtn);
    
    QVBoxLayout*    lay = new QVBoxLayout(this);
    lay -> addWidget(m_view, 1);
    lay -> addWidget(tb, 0);
    
    lay -> setContentsMargins(0,0,0,0);
    
    setWindowTitle("Directorys");
}

YDirectoryTable::~YDirectoryTable()
{
}

void    YDirectoryTable::check() 
{
    m_model -> check();
}

void      YDirectoryTable::cmdAdd()
{
    if(m_addEnable){
        //QString txt = m_addKey -> text().simplified();
        //if(txt.isEmpty())
            //return ;
        //Directory t   = cdb::make_tag(txt);
        //if(t)
            //m_model -> append(t);
    }
    
    m_addKey -> clear();
}

void      YDirectoryTable::doubleClicked(const QModelIndex&idx)
{
    if(is_in({"Key", "ID"}, m_model->columnKey(idx))){
        Directory t   = m_model -> get(idx.row());
        if(t)
            emit editReq(t.id);
    }
}

void            YDirectoryTable::refresh()
{
    m_model -> refresh();
}

void            YDirectoryTable::setAddEnable(bool f)
{
    m_addEnable = f;
    m_addKey -> setEnabled(f);
    m_addBtn -> setEnabled(f);
}


void   YDirectoryTable::setReadOnly(bool f)
{
    m_model -> setReadOnly(f);
}


