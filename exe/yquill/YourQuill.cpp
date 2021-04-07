////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "atom/YAtomTable.hpp"
#include "attribute/YAttributeTable.hpp"
#include "class/YClassTable.hpp"

#include "directory/YDirectoryTable.hpp"
#include "document/YDocumentTable.hpp"
#include "field/YFieldTable.hpp"
#include "folder/YFolderTable.hpp"
#include "fragment/YFragmentTable.hpp"
#include "property/YPropertyTable.hpp"
#include "leaf/YLeafTable.hpp"
#include "tag/YTagTable.hpp"

#include "YExplorer.hpp"
#include "YWeb.hpp"
#include "YourQuill.hpp"

#include <db/Workspace.hpp>
#include <dbgui/QuillApp.hpp>
#include <gadget/DateGen.hpp>
#include <gadget/SqlTool.hpp>
#include <gadget/TimeGen.hpp>
#include <gui/StdIcons.hpp>
#include <util/Logging.hpp>
#include <iostream>

#if 0
#include "ClassEditor.hpp"
#include "YClassTable.hpp"

#include "gadget/Calculator.hpp"
//#include "gadget/MonthView.hpp"
#include "gui/UInt64Mapper.hpp"

//#include <QApplication>
#include <QCursor>
#include <QMenu>
#include <QSignalMapper>
#endif

YourQuill::YourQuill() //: m_ctxClassMenu(nullptr)
{
    addAction("classes", "Classes").connect(this, &YourQuill::cmdNewClassTable);

    addAction("new_atomtable", "Atom Table").connect(this, &YourQuill::cmdNewAtomTable);
    addAction("new_attributetable", "Attribute Table").connect(this, &YourQuill::cmdNewAttributeTable);
    //addAction("new_calculator", "Calculator").connect(this, &YourQuill::cmdNewCalculator);
    addAction("new_classtable", "Class Table").connect(this, &YourQuill::cmdNewClassTable);
    addAction("new_dategen", "Date Generator").connect(this, &YourQuill::cmdNewDateGen);
    addAction("new_directorytable", "Directory Table").connect(this, &YourQuill::cmdNewDirectoryTable);
    addAction("new_documenttable", "Document Table").connect(this, &YourQuill::cmdNewDocumentTable);
    addAction("new_fieldstable", "Field Table").connect(this, &YourQuill::cmdNewFieldTable);
    addAction("new_foldertable", "Folder Table").connect(this, &YourQuill::cmdNewFolderTable);
    addAction("new_fragmenttable", "Fragment Table").connect(this, &YourQuill::cmdNewFragmentTable);
    addAction("new_explorer", "New Explorer").connect(this, &YourQuill::cmdNewExplorer);
    addAction("new_leaftable", "Leaf Table").connect(this, &YourQuill::cmdNewLeafTable);
    //addAction("new_month", "Month Calendar").connect(this, &YourQuill::cmdNewMonthView);
    addAction("new_proptable", "Property Table").connect(this, &YourQuill::cmdNewPropertyTable);
    addAction("new_sqltool", "Sql Tool").connect(this, &YourQuill::cmdNewSqlTool);
    addAction("new_tagtable", "Tag Table").connect(this, &YourQuill::cmdNewTagTable);
    addAction("new_timegen", "Time Generator").connect(this, &YourQuill::cmdNewTimeGen);
    addAction("new_web", "New Web").connect(this, &YourQuill::cmdNewWeb);
    
    addAction("quit", "Quit").icon(si_app_exit()).connect(&QApplication::closeAllWindows);
    
    addAction("refresh", "Refresh").shortcut("F5").icon(si_view_refresh());
    addAction("tags", "Tags").connect(this, &YourQuill::cmdNewTagTable);



    makeMenu("quill", "&YourQuill", QStringList() 
        << "new_web" 
        << "new_explorer"
        << "--"
        << "classes"
        << "tags"
        << "--"
        << "quit"
    );
    makeMenu("view", "&View", 
        QStringList() << "refresh"
    );
    makeMenu("gadget", "&Gadgets", 
        QStringList() 
            << "new_calculator" 
            << "new_dategen" 
            << "new_timegen" 
    );
    makeMenu("debug", "&Debug", 
        QStringList() 
            << "new_sqltool" 
            << "--"
            << "new_atomtable" 
            << "new_attributetable"
            << "new_classtable" 
            << "new_directorytable"
            << "new_documenttable" 
            << "new_fieldstable"
            << "new_foldertable"
            << "new_fragmenttable"
            << "new_leaftable" 
            << "new_proptable"
            << "new_tagtable"
        
    );

#if 0
    QStringList     edits, dels;
    UInt64Mapper*   mapEdit  = new UInt64Mapper(this);
    UInt64Mapper*   mapDel   = new UInt64Mapper(this);
    QAction*        act = nullptr;

    m_ctxClassMenu  = new QMenu;
    connect(mapEdit, &UInt64Mapper::mapped, this, &YourQuill::ctxCmdEdit);
    connect(mapDel, &UInt64Mapper::mapped, this, &YourQuill::ctxCmdDelete);
    addAction("ctx_atoms", "Atoms").connect(this, &YourQuill::ctxCmdAtoms);

    for(const Root* rt : wksp::roots()){
        QString         n   = QString("ctx_edit_%1").arg(rt->key());
        edits << n;
        act = addAction(n, tr("Edit %1").arg(rt->name())).connect(mapEdit, &UInt64Mapper::map);
        mapEdit -> setMapping(act, rt->id());
    
        n   = QString("ctx_delete_%1").arg(rt->key());
        dels << n;
        act = addAction(n, tr("Delete %1").arg(rt->name())).connect(mapDel, &UInt64Mapper::map);
        mapDel -> setMapping(act, rt->id());
    }
    
    addToMenu(m_ctxClassMenu, QStringList() << "ctx_atoms" << "--" << edits << "--" << dels);
#endif

    activateTabs();
    enableClosableTabs();
    enableDetachableTabs();
    enableAutoEnableCmds();

    resize(1920,1080);
    updateTitle();
    connect(this, &YourQuill::activeWindowChanged, this, &YourQuill::updateTitle);
}

YourQuill::~YourQuill()
{
}


void    YourQuill::cmdNewAtomTable()
{
    YAtomTable* ct  = new YAtomTable;
    addWindow(ct);
}


void    YourQuill::cmdNewAttributeTable()
{
    YAttributeTable* ct = new YAttributeTable;
    addWindow(ct);
}

#if 0

void    YourQuill::cmdNewCalculator()
{
    Calculator* c   = new Calculator;
    addDock(Qt::LeftDockWidgetArea, c);
}
#endif

void    YourQuill::cmdNewClassTable()
{
    YClassTable*     ct  = new YClassTable;
    ct -> setAddEnable(true);
    addWindow(ct);
}

void    YourQuill::cmdNewDateGen()
{
    DateGen *d  = new DateGen;
    addDock(Qt::BottomDockWidgetArea, d);
}

void    YourQuill::cmdNewDirectoryTable()
{
    YDirectoryTable*    ct  = new YDirectoryTable;
    addWindow(ct);
}

void    YourQuill::cmdNewDocumentTable()
{
    YDocumentTable*     ct = new YDocumentTable;
    addWindow(ct);
}

void    YourQuill::cmdNewExplorer()
{
    YExplorer*   e   = new YExplorer;
    addWindow(e);
}

void    YourQuill::cmdNewFieldTable()
{
    YFieldTable*    ft  = new YFieldTable;
    addWindow(ft);
}

void    YourQuill::cmdNewFolderTable()
{
    YFolderTable*   ft  = new YFolderTable;
    addWindow(ft);
}

void    YourQuill::cmdNewFragmentTable()
{
    YFragmentTable* ft  = new YFragmentTable;
    addWindow(ft);
}

void    YourQuill::cmdNewLeafTable()
{
    YLeafTable*     lt  = new YLeafTable;
    //lt -> setAddEnable(true);
    addWindow(lt);
}

#if 0

//void    YourQuill::cmdNewMonthView()
//{
    //MonthView   *m  = new MonthView;
    //addDock(Qt::LeftDockWidgetArea, m);
//}

#endif

void    YourQuill::cmdNewPropertyTable()
{
    YPropertyTable* pt = new YPropertyTable;
    addWindow(pt);
}

void    YourQuill::cmdNewSqlTool()
{
    SqlTool     *st = new SqlTool(wksp::cache());
    addWindow(st);
}

void    YourQuill::cmdNewTagTable()
{
    YTagTable*   tt  = new YTagTable;
    tt -> setAddEnable(true);
    addWindow(tt);
}

void    YourQuill::cmdNewTimeGen()
{
    TimeGen* t = new TimeGen;
    addDock(Qt::BottomDockWidgetArea, t);
}

void    YourQuill::cmdNewWeb()
{
    YWeb*        b   = new YWeb;
    addWindow(b);
    //  set - URL
}


#if 0
void    YourQuill::ctxClassTable(const QPoint& pt)
{
    YClassTable* ct  = qobject_cast<YClassTable*>(sender());
    if(!ct)
        return ;
    
    m_ctxClass  = ct -> classAt(pt);
    m_ctxClassMenu -> exec(QCursor::pos());
    m_ctxClass  = Class();
}

void    YourQuill::ctxCmdAtoms()
{
}

void    YourQuill::ctxCmdEdit(uint64_t i)
{
    const Root *rt  = wksp::root(i);
    if(!rt)
        return ;

    if(m_ctxClass.is_valid()){
        ClassEditor* ce = new ClassEditor(m_ctxClass, rt);
        addWindow(ce);
    }
}

void    YourQuill::ctxCmdDelete(uint64_t i)
{
    if(!wksp::root(i))
        return ;
}

#endif


void    YourQuill::reconnect(QWidget*w)
{
    MainWin::reconnect(w);
    connect(w, &QWidget::windowTitleChanged, this, &YourQuill::subWindowTitleChanged);
    
#if 0    
    YClassTable* ct  = qobject_cast<YClassTable*>(w);
    if(ct)
        connect(ct, &YClassTable::contextMenu, this, &YourQuill::ctxClassTable);
#endif
}

void    YourQuill::subWindowTitleChanged()
{
    if(activeWindow() == sender())
        updateTitle();
}

void    YourQuill::updateTitle()
{
    static const QString    name    = wksp::name();
    static const QString    abbr    = wksp::abbreviation();
    QWidget*w   = activeWindow();
    if(w)
        setWindowTitle(tr("%1 - %2 - YourQuill").arg(w->windowTitle()).arg(abbr));
    else
        setWindowTitle(tr("%1 - YourQuill").arg(name));
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

int main(int argc, char* argv[])
{
    log_to_std_error();
    QuillApp   app(argc, argv);
    meta_freeze();

    QString wp;
    if(argc>1)
        wp  = argv[1];
    
    //  We're going to workspace-prompt (later)
        
    if(!app.init_local_workspace(wp))
        return -1;
    
    YourQuill   *qw = new YourQuill;
    qw -> show();
    qw -> cmdNewExplorer();
    return app.exec();
}

#include "moc_YourQuill.cpp"

