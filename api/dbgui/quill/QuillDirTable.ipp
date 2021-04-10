////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////


#include "QuillDirTable.hpp"

#include <gui/StdIcons.hpp>
#include <gui/model/StdTableModelImpl.hpp>

#include <gui/delegate/ColorDelegate.hpp>
#include <gui/delegate/DirDelegate.hpp>
#include <gui/delegate/EnumDelegate.hpp>
#include <gui/delegate/StringDelegate.hpp>

#include <gui/validator/KeyValidator.hpp>
#include <util/Compare.hpp>

//#include "QuillEdit.hpp"

//#include "CopyrightEdit.hpp"

//#include "core/Set.hpp"

//#include "gui/DirBox.hpp"
//#include "gui/EnumDelegate.hpp"
//#include "gui/FileBox.hpp"
//#include "gui/LineEdit.hpp"
//#include "gui/SpinBox.hpp"

//#include "leaf/QuillDoc.hpp"

//#include <QFormLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QIcon>
#include <QToolBar>
#include <QVBoxLayout>

//enum {
    //qdtKey      = 0,
    //qdtPolicy   = 1
//};

//static const int    qdtFile   = qdtPolicy + DataRole::maxValue();
//static const int    qdtCount  = qdtFile + 1;

Q_DECLARE_METATYPE(DataRole)
Q_DECLARE_METATYPE(Access)

QuillDirTable::QuillDirTable(bool fTemplates, const QString& title_, QWidget*parent) : QGroupBox(title_, parent), 
    m_bar(nullptr), m_model(nullptr), m_view(nullptr)
{
    m_model     = new Model(fTemplates, this);
    m_view      = new View(m_model);
    
    m_bar       = new QToolBar;
    m_bar -> setOrientation(Qt::Vertical);
    
    m_bar -> addAction(si_list_up(), "^", this, &QuillDirTable::cmdMoveUp);
    m_bar -> addAction(si_list_down(), "v", this, &QuillDirTable::cmdMoveDown);
    m_bar -> addSeparator();
    m_bar -> addAction(si_list_add(), "+", this, &QuillDirTable::cmdAdd);
    m_bar -> addAction(si_list_remove(), "-", this, &QuillDirTable::cmdRemove);


    QVBoxLayout*    vlay    = new QVBoxLayout;
    vlay -> setContentsMargins(0,0,0,0);
    vlay->addStretch();
    vlay->addWidget(m_bar, 0);
    vlay->addStretch();

    QHBoxLayout*    hlay    = new QHBoxLayout(this);
    hlay -> setContentsMargins(0,0,0,0);
    hlay -> addWidget(m_view, 1);
    hlay -> addLayout(vlay, 0);
}

QuillDirTable::~QuillDirTable()
{
}

Vector<QString> QuillDirTable::dirs() const
{
    return m_model -> dirs();
}

void        QuillDirTable::get(Vector<QuillData::Root>&data) const
{
    m_model -> get(data);
}

void        QuillDirTable::set(const Vector<QuillData::Root>&data)
{
    m_model -> set(data);
}

void        QuillDirTable::addDir(const QString& v)
{
    QuillData::Root  d;
    d.path  = v;
    m_model -> append(d);
}

void        QuillDirTable::clear()
{
    m_model -> clear();
}

void        QuillDirTable::setReadOnly(bool f)
{
    m_bar -> setEnabled(!f);
    m_model -> setReadOnly(f);
}


void        QuillDirTable::cmdRemove()
{
    int i   = m_view -> selectedRow();
    if(i>=0)
        m_model -> removeRow(i);
}

void        QuillDirTable::cmdMoveDown()
{
    int i = m_view -> selectedRow();
    if(i>=0)
        m_model -> moveRowDown(i);
}

void        QuillDirTable::cmdMoveUp()
{
    int i = m_view -> selectedRow();
    if(i>=0)
        m_model -> moveRowUp(i);
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

template class StdTableModel<QuillData::Root>;

QuillDirTable::Model::Model(bool fTemplates, QObject*parent) : Base(parent)
{
    col("Key", &QuillData::Root::key).delegate<KeyValidator>();
    customRW("Color", [](const QuillData::Root&r) -> QColor {
        return QColor(r.color.qString());
    }, [](QuillData::Root& r, const QColor& c){
        r.color = c.name();
        return true;
    }).delegate<ColorDelegate>();
    
    for(DataRole dr : DataRole::all_values()){
        col(&QuillData::Root::policy, dr);
    }
    
    
    auto cw = col("Path", &QuillData::Root::path);
    if(!fTemplates)
        cw.delegate<DirDelegate>();
}

QuillDirTable::Model::~Model()
{
}


Vector<QString>     QuillDirTable::Model::dirs() const
{
    Vector<QString> ret;
    for(auto& r : m_rows)
        ret << r.data.path.qString();
    return ret;
}


void        QuillDirTable::Model::get(Vector<QuillData::Root>&data) const
{
    data.clear();
    for(auto& r : m_rows)
        data << r.data;
}



void        QuillDirTable::Model::set(const Vector<QuillData::Root>&data)
{
    beginResetModel();
    Base::append(data);
    endResetModel();
}



//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

QuillDirTable::View::View(Model*m) : m_model(m)
{
    setModel(m);
    setSelectionBehavior(SelectRows);
    setSelectionMode(SingleSelection);
    horizontalHeader()->setStretchLastSection(true);
    
    for(auto c : m->columns()){
        Delegate*d  = c->delegate(this);
        if(d)
            setItemDelegateForColumn(c->id(), d);
    }
}

QuillDirTable::View::~View()
{
}


int         QuillDirTable::View::selectedRow() const
{
    QModelIndexList s   = selectedIndexes();
    if(s.isEmpty())
        return -1;
    return s.front().row();
}

