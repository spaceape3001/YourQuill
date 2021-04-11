////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

YLeafTable::YLeafTable(LeafProvider stp, QWidget*parent) : SubWin(parent), m_model{}, m_view{}, m_addKey{}, m_addBtn{}, m_addEnable{}
{
    if(!stp)
        stp     = provider::all_leafs();
        
    m_model     = new LeafTableModel(stp, this);
    
    m_model -> setReadOnly(false);
    m_view      = new LeafTableView(m_model);
    
    QToolBar*   tb  = new QToolBar;
    m_addKey        = new QLineEdit;
    m_addKey -> setValidator(new KeyValidator(m_addKey));
    m_addBtn        = new QPushButton("+");
    setAddEnable(false);
    
    connect(m_addKey, &QLineEdit::returnPressed, this, &YLeafTable::cmdAdd);
    connect(m_addBtn, &QPushButton::clicked, this, &YLeafTable::cmdAdd);
    
    connect(m_view, &QTableView::doubleClicked, this, &YLeafTable::doubleClicked);
    
    tb->addWidget(m_addKey);
    tb->addWidget(m_addBtn);
    
    QVBoxLayout*    lay = new QVBoxLayout(this);
    lay -> addWidget(m_view, 1);
    lay -> addWidget(tb, 0);
    
    lay -> setContentsMargins(0,0,0,0);
    
    setWindowTitle("Leafs");
}

YLeafTable::~YLeafTable()
{
}

void    YLeafTable::check() 
{
    m_model -> check();
}

void      YLeafTable::cmdAdd()
{
    if(m_addEnable){
    #if 0
        QString txt = m_addKey -> text().simplified();
        if(txt.isEmpty())
            return ;
        Leaf t   = cdb::make_tag(txt);
        if(t)
            m_model -> append(t);
    #endif
    }
    
    m_addKey -> clear();
}

void      YLeafTable::doubleClicked(const QModelIndex&idx)
{
    if(is_in({"Key", "ID"}, m_model->columnKey(idx))){
        Leaf t   = m_model -> get(idx.row());
        if(t)
            emit editReq(t.id);
    }
}

void            YLeafTable::refresh()
{
    m_model -> refresh();
}

void            YLeafTable::setAddEnable(bool f)
{
    m_addEnable = f;
    m_addKey -> setEnabled(f);
    m_addBtn -> setEnabled(f);
}


void   YLeafTable::setReadOnly(bool f)
{
    m_model -> setReadOnly(f);
}


