////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////


#include "QuillDirTable.hpp"
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
    
    m_bar -> addAction(QIcon(":/leaf/up.svg"), "^", this, &QuillDirTable::cmdMoveUp);
    m_bar -> addAction(QIcon(":/leaf/down.svg"), "v", this, &QuillDirTable::cmdMoveDown);
    m_bar -> addSeparator();
    m_bar -> addAction(QIcon(":/leaf/add.svg"), "+", this, &QuillDirTable::cmdAdd);
    m_bar -> addAction(QIcon(":/leaf/remove.svg"), "-", this, &QuillDirTable::cmdRemove);


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

QuillDirTable::Model::Model(bool fTemplates, QObject*parent) : Base(parent)
{
    col("Key", &QuillData::Root::key).delegate<KeyValidator>();
    col("Color", &QuillData::Root::color).delegate<ColorDelegate>();
    
    for(DataRole dr : DataRole::all_values()){
        col(&QuillData::Root::policy, dr);
    }
    
    
    auto cw = col("Path", &QuillData::Root::path);
    if(!fTemplates)
        cw.delegate<DirDelegate>();
    

    //Column  col{};
    
    //col.k       = "key";
    //col.label   = tr("Key");
    //col.display = [](const Column&, const QuillData::Root& r) -> QVariant {
        //return r.key.qString();
    //};
    //col.set     = [](const Column&, const QVariant& v, QuillData::Root& r) -> bool {
        //r.key   = String(v.toString()).strip_spaces();
        //return true;
    //};
    //col.less    = [](const Column&, const QuillData::Root& a, const QuillData::Root& b) -> bool {
        //return is_less(compare_igCase(a.key, b.key));
    //};
    //addColumn(col);
    
    //col         = Column{};
    //col.k       = "name";
    //col.label   = tr("Name");
    //col.display = [](const Column&, const QuillData::Root&r) -> QVariant {
        //return r.name.qString();
    //};
    //col.set     = [](const Column&, const QVariant& v, QuillData::Root& r) -> bool {
        //r.name  = String(v.toString()).strip_spaces();
        //return true;
    //};
    //col.less    = [](const Column&, const QuillData::Root&a, const QuillData::Root& b) -> bool {
        //return is_less(compare_igCase(a.name, b.name));
    //};
    
    //col     = Column{};
    //col.k       = "color";
    //col.label   = tr("Color");
    //col.display = [](const Column&, const QuillData::Root& r) -> QVariant {
        //return r.color.qString();
    //};
    //col.set     = [](const Column&, const QVariant&v, QuillData::Root& r) -> bool {
        //r.color = String(v.toString()).strip_spaces();
        //return true;
    //};
    //addColumn(col);
    
    //for(DataRole dr : DataRole::all_values()){
        //col = Column{};
        //col.k       = dr.key().qString();
        //col.label   = dr.key().qString();
        //col.data    = Variant(dr);
        //col.display = [](const Column& c, const QuillData::Root& r) -> QVariant {
            //return QVariant::fromValue(r.policy[ c.data.value<DataRole>()]);
        //};
        //col.set    = [](const Column& c, const QVariant& v, QuillData::Root& r) -> bool {
            //r.policy[c.data.value<DataRole>()] = v.value<Access>();
            //return true;
        //};
        //col.less    = [](const Column&c, const QuillData::Root&a, const QuillData::Root&b) -> bool {
            //DataRole    dr  = c.data.value<DataRole>();
            //return a.policy[dr] < b.policy[dr];
        //};
        //addColumn(col);
    //}
    
    //col         = Column{};
    //col.k       = tr("path");
    //col.label   = tr("Directory");
    //col.display = [](const Column&, const QuillData::Root& r) -> QVariant {
        //return r.path.qString();
    //};
    //col.set     = [](const Column&, const QVariant& v, QuillData::Root&r) -> bool {
        //r.path  = String(v.toString()).strip_spaces();
        //return true;
    //};
    //col.less    = [](const Column&, const QuillData::Root& a, const QuillData::Root& b) -> bool {
        //return a.path < b.path; // TODO: Windows
    //};
    //addColumn(col);

}

QuillDirTable::Model::~Model()
{
}


Vector<QString>     QuillDirTable::Model::dirs() const
{
    Vector<QString> ret;
    for(auto& r : m_rows)
        ret << r.path.qString();
    return ret;
}


void        QuillDirTable::Model::get(Vector<QuillData::Root>&data) const
{
    data  = m_rows;
}



void        QuillDirTable::Model::set(const Vector<QuillData::Root>&data)
{
    beginResetModel();
    m_rows = data;
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

#include "moc_QuillDirTable.cpp"
