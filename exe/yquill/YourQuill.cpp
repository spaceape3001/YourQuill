////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "YTagTable.hpp"
#include "YWeb.hpp"
#include "YourQuill.hpp"

#include <db/Workspace.hpp>
#include <dbgui/QuillApp.hpp>
#include <gadget/DateGen.hpp>
#include <gadget/TimeGen.hpp>
#include <util/Logging.hpp>
#include <iostream>

#if 0
#include "ClassEditor.hpp"
#include "ClassTable.hpp"
#include "Explorer.hpp"
#include "YWeb.hpp"

#include "gadget/Calculator.hpp"
//#include "gadget/MonthView.hpp"
#include "gadget/SqlTool.hpp"
#include "gui/UInt64Mapper.hpp"

//#include <QApplication>
#include <QCursor>
#include <QMenu>
#include <QSignalMapper>
#include <QString>
#endif

YourQuill::YourQuill() //: m_ctxClassMenu(nullptr)
{
    //addAction("new_calculator", "Calculator").connect(this, &YourQuill::cmdNewCalculator);
    //addAction("new_classtable", "Classes").connect(this, &YourQuill::cmdNewClassTable);
    addAction("new_dategen", "Date Generator").connect(this, &YourQuill::cmdNewDateGen);
    //addAction("new_explorer", "New Explorer").connect(this, &YourQuill::cmdNewExplorer);
    //addAction("new_month", "Month Calendar").connect(this, &YourQuill::cmdNewMonthView);
    //addAction("new_sqltool", "Sql Tool").connect(this, &YourQuill::cmdNewSqlTool);
    addAction("new_tagtable", "Tags").connect(this, &YourQuill::cmdNewTagTable);
    addAction("new_timegen", "Time Generator").connect(this, &YourQuill::cmdNewTimeGen);
    addAction("new_web", "New Web").connect(this, &YourQuill::cmdNewWeb);
    
    addAction("refresh", "Refresh").shortcut("F5");


    makeMenu("quill", "&YourQuill", QStringList() << "new_web" << "new_explorer");
    makeMenu("view", "&View", QStringList() << "refresh");
    makeMenu("table", "&Table", QStringList() << "new_classtable" << "new_tagtable");
    makeMenu("gadget", "&Gadgets", QStringList() << "new_calculator" << "new_dategen" << "new_timegen");
    makeMenu("debug", "&Debug", QStringList() << "new_sqltool" );

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

#if 0

void    YourQuill::cmdNewCalculator()
{
    Calculator* c   = new Calculator;
    addDock(Qt::LeftDockWidgetArea, c);
}

void    YourQuill::cmdNewClassTable()
{
    ClassTable*     ct  = new ClassTable;
    ct -> setAddEnable(true);
    addWindow(ct);
}
#endif

void    YourQuill::cmdNewDateGen()
{
    DateGen *d  = new DateGen;
    addDock(Qt::BottomDockWidgetArea, d);
}

#if 0
void    YourQuill::cmdNewExplorer()
{
    Explorer*   e   = new Explorer;
    addWindow(e);
}

//void    YourQuill::cmdNewMonthView()
//{
    //MonthView   *m  = new MonthView;
    //addDock(Qt::LeftDockWidgetArea, m);
//}

void    YourQuill::cmdNewSqlTool()
{
    SqlTool     *st = new SqlTool(wksp::cache());
    addWindow(st);
}
#endif

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
    ClassTable* ct  = qobject_cast<ClassTable*>(sender());
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
    ClassTable* ct  = qobject_cast<ClassTable*>(w);
    if(ct)
        connect(ct, &ClassTable::contextMenu, this, &YourQuill::ctxClassTable);
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
    qw -> cmdNewWeb();
    return app.exec();
}

#include "moc_YourQuill.cpp"

