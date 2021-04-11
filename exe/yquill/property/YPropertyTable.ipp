////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

YPropertyTable::YPropertyTable(PropertyProvider stp, QWidget*parent) : SubWin(parent), m_model{}, m_view{}, m_addKey{}, m_addBtn{}, m_addEnable{}
{
    if(!stp)
        stp     = provider::all_properties();
        
    m_model     = new PropertyTableModel(stp, this);
    
    m_model -> setReadOnly(false);
    m_view      = new PropertyTableView(m_model);
    
    QToolBar*   tb  = new QToolBar;
    m_addKey        = new QLineEdit;
    m_addKey -> setValidator(new KeyValidator(m_addKey));
    m_addBtn        = new QPushButton("+");
    setAddEnable(false);
    
    connect(m_addKey, &QLineEdit::returnPressed, this, &YPropertyTable::cmdAdd);
    connect(m_addBtn, &QPushButton::clicked, this, &YPropertyTable::cmdAdd);
    
    connect(m_view, &QTableView::doubleClicked, this, &YPropertyTable::doubleClicked);
    
    tb->addWidget(m_addKey);
    tb->addWidget(m_addBtn);
    
    QVBoxLayout*    lay = new QVBoxLayout(this);
    lay -> addWidget(m_view, 1);
    lay -> addWidget(tb, 0);
    
    lay -> setContentsMargins(0,0,0,0);
    
    setWindowTitle("Propertys");
}

YPropertyTable::~YPropertyTable()
{
}

void    YPropertyTable::check() 
{
    m_model -> check();
}

void      YPropertyTable::cmdAdd()
{
    if(m_addEnable){
        //QString txt = m_addKey -> text().simplified();
        //if(txt.isEmpty())
            //return ;
        //Property t   = cdb::make_tag(txt);
        //if(t)
            //m_model -> append(t);
    }
    
    m_addKey -> clear();
}

void      YPropertyTable::doubleClicked(const QModelIndex&idx)
{
    if(is_in({"Key", "ID"}, m_model->columnKey(idx))){
        Property t   = m_model -> get(idx.row());
        if(t)
            emit editReq(t.id);
    }
}

void            YPropertyTable::refresh()
{
    m_model -> refresh();
}

void            YPropertyTable::setAddEnable(bool f)
{
    m_addEnable = f;
    m_addKey -> setEnabled(f);
    m_addBtn -> setEnabled(f);
}


void   YPropertyTable::setReadOnly(bool f)
{
    m_model -> setReadOnly(f);
}

