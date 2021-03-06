////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "QuillDataEdit.hpp"
#include "QuillDirTable.hpp"
#include "QuillEdit.hpp"


#include <db/QuillData.hpp>
#include <util/Set.hpp>

//#include "gui/DirBox.hpp"
//#include "gui/EnumDelegate.hpp"
//#include "gui/FileBox.hpp"
//#include "gui/LineEdit.hpp"
//#include "gui/SpinBox.hpp"


//#include <QFormLayout>
//#include <QHBoxLayout>
//#include <QHeaderView>
//#include <QIcon>
//#include <QToolBar>
//#include <QVBoxLayout>


QuillEdit::QuillEdit(QWidget*parent) : QTabWidget(parent), m_data(nullptr), m_roots(nullptr), m_templates(nullptr)
{
    m_data      = new QuillDataEdit;
    m_roots     = new QuillDirTable(false, tr("Directories"));
    connect(m_roots, &QuillDirTable::cmdAdd, this, &QuillEdit::cmdAddRootDir);
    m_templates = new QuillDirTable(true, tr("Templates"));
    connect(m_templates, &QuillDirTable::cmdAdd, this, &QuillEdit::cmdAddTemplate);
    
    addTab(m_data, tr("Details"));
    addTab(m_templates, tr("Templates"));
    addTab(m_roots, tr("Other Directories"));
}

QuillEdit::~QuillEdit()
{
}


void        QuillEdit::addRoot(const QString&v)
{
    m_roots -> addDir(v);
}

void        QuillEdit::addTemplate(const QString&v)
{
    m_templates -> addDir(v);
}

void        QuillEdit::clear()
{
    m_data -> clear();
    m_roots -> clear();
    m_templates -> clear();
}

void        QuillEdit::get(QuillData&doc) const
{
    m_data -> get(doc);
    m_roots -> get(doc.roots);
    m_templates -> get(doc.templates);
}

void        QuillEdit::set(const QuillData&doc)
{
    m_data -> set(doc);
    m_roots -> set(doc.roots);
    m_templates -> set(doc.templates);
}

QStringSet   QuillEdit::templates() const
{
    QStringSet   ret;
    for(QString s : m_templates -> dirs())
        ret << s;
    return ret;
}



#include "moc_QuillEdit.cpp"
