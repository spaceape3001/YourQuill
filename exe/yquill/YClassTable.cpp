////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "YClassTable.hpp"
#include <db/Workspace.hpp>
#include <dbgui/atomsys/ClassTableModel.hpp>
#include <dbgui/atomsys/ClassTableView.hpp>
#include <dbgui/wksp/RootComboBox.hpp>
#include <gui/edit/LineEdit.hpp>
#include <gui/validator/KeyValidator.hpp>
#include <util/Utilities.hpp>

#include <QIcon>
#include <QPushButton>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>

YClassTable::YClassTable(ClassProvider cpp, QWidget* parent) : SubWin(parent), m_model(nullptr), 
    m_view(nullptr), m_addRoot{}, m_addKey{}, m_addBtn{}, m_addEnable{}
{
    if(!cpp)
        cpp = provider::all_classes();
    
    m_model = new ClassTableModel(cpp, this);
    m_model -> setReadOnly(false);
    //m_model -> setColumns({"id", "key", "roots", "name", "plural", "brief"});
    //m_colID     = m_model -> column("id");
    //m_colKey    = m_model -> column("key");
    
    m_view = new ClassTableView(m_model);
    m_view -> setContextMenuPolicy(Qt::CustomContextMenu);
    
    QToolBar*   tb  = new QToolBar;
    
    m_addRoot       = new RootComboBox;
    m_addRoot -> setDataRole(DataRole::Classes);
    m_addRoot -> setWritableRoots(true);
    
    m_addKey        = new LineEdit;
    m_addKey -> setValidator(new KeyValidator(m_addKey));
    m_addKey -> setPlaceholderText("(new class key)");
    m_addBtn    = new QPushButton("+");
    setAddEnable(false);
    
    connect(m_addKey, &LineEdit::returnPressed, this, &YClassTable::cmdAdd);
    connect(m_addBtn, &QPushButton::clicked, this, &YClassTable::cmdAdd);
    
    connect(m_view, &QTableView::doubleClicked, this, &YClassTable::doubleClicked);
    connect(m_view, &QWidget::customContextMenuRequested, this, &YClassTable::contextMenu);
    
    tb->addWidget(m_addRoot);
    tb->addWidget(m_addKey);
    tb->addWidget(m_addBtn);
    
    QVBoxLayout*    lay = new QVBoxLayout(this);
    lay -> setContentsMargins(0,0,0,0);
    lay -> addWidget(m_view, 1);
    lay -> addWidget(tb, 0);
    setWindowTitle("Classes");
}

YClassTable::~YClassTable()
{
}

void   YClassTable::check() 
{
    m_model -> check();
}

Class  YClassTable::classAt(const QPoint&pt) const
{
    QModelIndex idx     = m_view -> indexAt(pt);
    return m_model -> getClass(idx);
}


void   YClassTable::cmdAdd()
{
    do {
        if(!m_addEnable)
            break;
        QString     txt = m_addKey -> text().simplified();
        if(txt.isEmpty())
            break;
        const Root* rt  = m_addRoot -> currentRoot();
        if(!rt)
            rt  = wksp::root_first(DataRole::Classes);
        if(!rt)
            break;
        Class   c   = cdb::make_class(txt, rt);
        if(c)
            m_model -> append(c);
    } while(false); 
    m_addKey -> clear();
}

void      YClassTable::doubleClicked(const QModelIndex&idx)
{
    if(is_in({"ID", "Key"}, m_model -> columnKey(idx))){
        Class t   = m_model -> getClass(idx);
        if(t)
            emit editReq(t.id);
    }
}


bool   YClassTable::readOnly() const
{
    return m_model -> readOnly();
}

void   YClassTable::refresh() 
{
    m_model -> refresh();
}

void   YClassTable::setAddEnable(bool f)
{
    m_addEnable = f;
    m_addKey -> setEnabled(f);
    m_addBtn -> setEnabled(f);
    m_addRoot -> setEnabled(f);
}

void   YClassTable::setReadOnly(bool f)
{
    m_model -> setReadOnly(f);
}

#include "moc_YClassTable.cpp"

