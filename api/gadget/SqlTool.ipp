////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once


SqlTool::SqlTool(QSqlDatabase db, QWidget*parent) : SubWin(parent), 
    m_model(nullptr), m_view(nullptr), m_edit(nullptr), 
    m_error(nullptr), m_stack(nullptr), m_query(db)
{
    m_model     = new QSqlQueryModel(this);
    m_view      = new QTableView(this);
    m_edit      = new QComboBox(this);
    m_edit -> setEditable(true);
    m_edit -> setMaxCount(50);
    m_edit -> setInsertPolicy(QComboBox::InsertAtTop);
    
    m_error     = new QTextEdit(this);
    m_stack     = new QStackedLayout;
    QPushButton*    rBtn    = new QPushButton(tr("Refresh"), this);
    QPushButton*    sBtn  = new QPushButton(tr("Submit"), this);
    QPushButton*    cBtn   = new QPushButton(tr("Clear"), this);
    
    QVBoxLayout*    lay = new QVBoxLayout(this);
    QHBoxLayout*    line    = new QHBoxLayout;
    
    line -> addWidget(rBtn, 0);
    line -> addWidget(m_edit, 1);
    line -> addWidget(sBtn, 0);
    line -> addWidget(cBtn, 0);
    m_stack -> addWidget(m_error);
    m_stack -> addWidget(m_view);
    
    m_view -> setModel(m_model);
    m_view -> setAlternatingRowColors(true);
    m_view -> setSortingEnabled(true);
    
    lay -> addLayout(line, 0);
    lay -> addLayout(m_stack, 1);
    
    connect(sBtn, &QPushButton::clicked, this, &SqlTool::submit);
    connect(cBtn, &QPushButton::clicked, this, &SqlTool::clear);
    connect(rBtn, &QPushButton::clicked, this, &SqlTool::refresh);
    connect(m_edit->lineEdit(), &QLineEdit::returnPressed, this, &SqlTool::submit);
    
    setWindowTitle(tr("SQL Tool"));
    
}

SqlTool::~SqlTool()
{
}

void        SqlTool::_exec()
{
    if(m_query.exec()){
        m_model -> setQuery(m_query);
        m_stack -> setCurrentWidget(m_view);
    } else {
        m_error->setPlainText( m_query.lastError().text());
        m_stack -> setCurrentWidget(m_error);
    }
}

void        SqlTool::clear()
{
    m_model -> clear();
    m_edit -> clear();
    m_error -> clear();
}

void        SqlTool::refresh()
{
    if(!m_query.isSelect()) //  lets not repeat an update!
        return ;
        
    _exec();
}

void        SqlTool::submit()
{
    QString s = m_edit -> currentText().trimmed();
    if(!s.isEmpty() && !s.startsWith("--")){
        if(!m_query.prepare(s)){
            m_error->setPlainText( m_query.lastError().text());
            m_stack -> setCurrentWidget(m_error);
        } else {
            _exec();
        }
    }
}

