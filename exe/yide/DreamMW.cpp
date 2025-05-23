////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "DreamMW.hpp"

#include <yq/core/Logging.hpp>
#include <yq/container/Map.hpp>
#include <yq/container/Vector.hpp>
#include <gluon/core/Logging.hpp>
#include <gluon/core/Utilities.hpp>
#include <gluon/util/UInt64SignalMapper.hpp>

#include "Action.hpp"
#include "Command.hpp"
#include "Dock.hpp"
#include "WebBrowser.hpp"
#include "Window.hpp"

#include <mithril/wksp/Workspace.hpp>
#include <gluon/core/Utilities.hpp>

#include <QCursor>
#include <QMenu>
#include <QMenuBar>

using namespace yq;
using namespace yq::gluon;
using namespace yq::mithril;

struct KCmdData {
    const Command*  cmd = nullptr;
    QAction*        act = nullptr;
};


namespace {

    std::vector<const Command*> make_commands()
    {
        std::vector<const Command*>     ret;
        for(const Command* cmd : Command::all()){
            if(!cmd)
                continue;
            ret.push_back(cmd);
        }

        std::sort(ret.begin(), ret.end(), [](const Command* a, const Command* b) -> bool {
            if(a->gravity() < b->gravity())
                return true;
            if(a->gravity() > b->gravity())
                return false;
            return QString::compare(a->label(), b->label(), Qt::CaseInsensitive) < 0;
        });
        
        return ret;
    }

    const std::vector<const Command*>& commands()
    {
        static const std::vector<const Command*>    s_ret   = make_commands();
        return s_ret;
    }
    

}

DreamMW::DreamMW()
{
    activateTabs();
    enableClosableTabs();
    enableDetachableTabs();
    enableAutoEnableCmds();
    
    resize(1920,1080);
    updateTitle();
    
    std::vector<QAction*>   dockActions;
    for(const auto& di : ZDock::all()){
    
        ZDock*   d   = di.fnCreate();
        
        ActionInfo  ai  = di.action;
        ai.checkable    = true;
        
        QAction* act    = createAction(ai);
        d->m_action = act;
        d->setVisible(ai.checked);
        d->setAllowedAreas(di.allowedAreas);
        d->setWindowTitle(ai.label);

        connect(act, &QAction::triggered, d, &ZDock::triggered);
        addDock(di.startArea, d);
        dockActions.push_back(act);
    }
    
    std::sort(dockActions.begin(), dockActions.end(), [](const QAction*a, const QAction*b)->bool {
        return QString::compare(a->text(), b->text(), Qt::CaseInsensitive) < 0;
    });
    
    
    m_cmdActions.resize(Command::count(), nullptr);
    
    UInt64SignalMapper*     cmdMapper   = new UInt64SignalMapper(this);
    connect(cmdMapper, &UInt64SignalMapper::mapped, this, &DreamMW::commandTriggered);
    
    for(const Command *cmd : commands()){
        QAction*    act = createAction(cmd->action());
        connect(act, &QAction::triggered, cmdMapper, &UInt64SignalMapper::map);
        cmdMapper->setMapping(act, cmd->id());
        m_cmdActions[cmd->id()]     = act;
    }
    
    addAction("browser", "New Browser").connect(this, &DreamMW::newBrowser);
    addAction("copy", "Copy").icon(QIcon(":/icon/copy.svg")).shortcut(QKeySequence::Copy);
    addAction("cut", "Cut").icon(QIcon(":/icon/cut.svg")).shortcut(QKeySequence::Cut);
    addAction("clear", "Clear").icon(QIcon(":/icon/clear.svg")).shortcut(QKeySequence::Delete);
    addAction("find", "Find").icon(QIcon(":/icon/find.svg")).shortcut(QKeySequence::Find);
    addAction("paste", "Paste").icon(QIcon(":/icon/paste.svg")).shortcut(QKeySequence::Paste);
    addAction("print", "Print").icon(QIcon(":/icon/print.svg")).shortcut(QKeySequence::Print);
    addAction("redo", "Redo").icon(QIcon(":/icon/redo.svg")).shortcut(QKeySequence::Redo);
    addAction("refresh", "Refresh").icon(QIcon(":/icon/refresh.svg")).shortcut(QKeySequence::Refresh);
    addAction("save", "Save").icon(QIcon(":/icon/save.svg")).shortcut(QKeySequence::Save);
    addAction("undo", "Undo").icon(QIcon(":/icon/undo.svg")).shortcut(QKeySequence::Undo);
    addAction("zoomIn", "Zoom In").icon(QIcon(":/icon/zoomIn.svg")).shortcut(QKeySequence::ZoomIn);
    addAction("zoomOut", "Zoom In").icon(QIcon(":/icon/zoomOut.svg")).shortcut(QKeySequence::ZoomOut);
    
    
    //  popups....
    UInt64SignalMapper*     popupMapper   = new UInt64SignalMapper(this);
    connect(popupMapper, &UInt64SignalMapper::mapped, this, &DreamMW::popupCommand);
    
    for(uint64_t i=0;i<(uint64_t) Command::Type::COUNT; ++i){
        PopupItem   pi;
        if(i){
            pi.act  = new QAction(this);
            connect(pi.act, &QAction::triggered, popupMapper, &UInt64SignalMapper::map);
            popupMapper->setMapping(pi.act, i);
        }
        m_popupItems.push_back(pi);
    }
    
    m_popupItems[(size_t) Command::Type::View      ].separator  = true;
    m_popupItems[(size_t) Command::Type::Delete    ].separator  = true;
    m_popupItems[(size_t) Command::Type::Delete    ].last       = true;
    m_popupItems[(size_t) Command::Type::Properties].separator  = true;
    
    
    QMenu* fileMenu     = makeMenu("file", "File");
    QMenu* editMenu     = makeMenu("edit", "Edit");
    QMenu* viewMenu     = makeMenu("view", "View");
    QMenu* studioMenu   = makeMenu("studio", "Studio");
    QMenu* windowMenu   = makeMenu("window", "Window");
    QMenu* helpMenu     = makeMenu("help", "Help");
    
    menuBar()->addSeparator();
    QMenu* debugMenu    = makeMenu("debug", "Debug");
    
        //  ALL THOSE *BEFORE* automatics

    addToMenu(fileMenu,
        QStringList()
            << "save"
            << "--"
    );

    addToMenu(editMenu,
        QStringList()
            << "undo"
            << "redo"
            << "--"
            << "cut"
            << "copy"
            << "paste"
            << "clear"
            << "--"
            << "find"
    );

    for(QAction* act : dockActions)
        viewMenu -> addAction(act);

    addToMenu(viewMenu,
        QStringList()
    );
        
    addToMenu(studioMenu,
        QStringList() 
            << "browser" 
            << "--"
    );
    
    addToMenu(windowMenu,
        QStringList()
    );
    
    addToMenu(helpMenu,
        QStringList()
    );
    
    addToMenu(debugMenu,
        QStringList()
    );
    

        //  COMMANDS
        

    //  Add them to the menu... if advertised and argless...
    for(const Command* cmd : commands()){
        if(cmd->flavor() == ArgFlavor::Id) // requires id-argument, skip
            continue;
        if(cmd->menu().isEmpty())       // no menu... popup only
            continue;
        
        QMenu*  m   = menu(cmd->menu());
        if(!m)
            continue;
        m->addAction(m_cmdActions[cmd->id()]);
    }
    
        // POST-PEND

    addToMenu(fileMenu,
        QStringList() 
            << "print"
    );

    addToMenu(viewMenu,
        QStringList()
            << "--"
            << "zoomIn"
            << "zoomOut"
            << "refresh"
    );
    
    
    addToMenu(windowMenu,
        QStringList()
    );
 
    addToMenu(helpMenu,
        QStringList()
    );

    addToMenu(debugMenu,
        QStringList()
    );

    //  I do *NOT* know why this isn't working on X11.  
    //  Qt will otherwise render it, but *NOT* as a window icon. 
    //  Figure it's something in the file, but I know not what that is.
    //  However, it's distracting from what I'm working on
    setWindowIcon(QIcon(":/yquill.svg"));
}

DreamMW::~DreamMW()
{
}

QDockWidget*    DreamMW::addDock(Qt::DockWidgetArea dwa, QWidget*q)
{
    QDockWidget* dw = MainWindow::addDock(dwa, q);
    
    ZDock*   d   = qobject_cast<ZDock*>(q);
    if(d){
        connect(d, &ZDock::popupRequested, this, &DreamMW::popupRequested);
        connect(d, &ZDock::openRequested,  this, &DreamMW::openRequested);
    }
    return dw;
}

void    DreamMW::commandTriggered(uint64_t id)
{
    const Command*  cmd = Command::get(id);
    if(cmd){
        if(cmd->accept(m_idForCmd))
            cmd->invoke(this, m_idForCmd);
    }
    m_idForCmd      = Id();
}

void    DreamMW::newBrowser()
{
    addWindow(new WebBrowser);
}

MainWindow*  DreamMW::newMain()
{
    return new DreamMW;
}

void    DreamMW::openRequested(Id i)
{
    const Command*  edit    = nullptr;
    const Command*  view    = nullptr;
    
    for(const Command* cmd : commands()){
        if(cmd -> flavor() != ArgFlavor::Id)    // command's not ID triggered... cycle
            continue;
        
        switch(cmd->type()){
        case Command::Type::Edit:
            if(edit)
                continue;
            break;
        case Command::Type::View:
            if(view)
                continue;
            break;
        default:
            continue;
        }
        
        if(!cmd -> accept(i))                   // give the command a chance to reject
            continue;
            
        if(cmd->type() == Command::Type::Edit)
            edit    = cmd;
        else
            view    = cmd;
            
        cmd -> invoke(this, i);
        return;
    }

        // if both view & edit, let gravity override
    if(edit && view && (view->gravity() < edit->gravity()))
        edit    = nullptr;
    
    if(edit){
        edit -> invoke(this, i);
    } else if(view){
        view -> invoke(this, i);
    } else {
        yInfo() << "Unable to open " << i.type_name() << ":" << i.id();
    }
}

struct PopupHelper {
    QMenu*      menu        = nullptr;
    bool        separator   = false;
    bool        triggered   = false;
    
    PopupHelper() 
    {
        menu        = new QMenu;
    }
    
    ~PopupHelper()
    {
        menu -> deleteLater();
    }
    
    void    add(QAction* act)
    {
        if(separator){
            menu -> addSeparator();
            separator = false;
        }
        menu -> addAction(act);
        triggered   = true;
    }
};

void    DreamMW::popupCommand(uint64_t n)
{
    if(n >= m_popupItems.size())
        return ;
    
    PopupItem& pi   = m_popupItems[n];
    if(pi.cmd && m_idForCmd)
        pi.cmd -> invoke(this, m_idForCmd);
}


void    DreamMW::popupRequested(Id i, const QActionList& useActs)
{
    /*
        A popup has been requested by the user (in wherever).  
        As this menu's dynamic, we'll be forming it dynamically.
    */

    // Set the ID for all subsequent actions
    m_idForCmd      = i;
    
    for(auto& pi : m_popupItems)
        pi.cmd      = nullptr;
    
    // The menu
    PopupHelper     popup;
    
    
    // Need to know the actions to populate (between view/edit & delete)
    std::vector<QAction*>   actions;
    
    for(const Command* cmd : commands()){
        if(cmd -> flavor() != ArgFlavor::Id)    // command's not ID triggered... cycle
            continue;
        if(!cmd -> accept(i))                   // give the command a chance to reject
            continue;
        PopupItem& pi = m_popupItems[(size_t) cmd->type()];
        if(pi.act && !pi.cmd){
            pi.cmd  = cmd;
            pi.act -> setText(QString("%2 %1").arg(qString(i.name())).arg(cmd->label()));
            continue;
        }

        //  if it reaches here... it'll go on the list
        actions.push_back(m_cmdActions[cmd->id()]);
    }
    
    
    //  Now we'll add the items, start with view & edit
    for(PopupItem& pi : m_popupItems){
        if(pi.last)
            break;
        if(pi.separator)
            popup.separator = true;
        if(pi.cmd && pi.act)
            popup.add(pi.act);
    }

    popup.separator = true;
    
    for(QAction* a : useActs)
        popup.add(a);

    popup.separator = true;
    
    //  Add the non-conforming options
    for(QAction* a : actions)
        popup.add(a);
    
    popup.separator = true;
    
    bool    inLast  = false;
    for(PopupItem& pi : m_popupItems){
        if(pi.last)
            inLast  = true;
        if(!inLast)
            continue;
        if(pi.separator)
            popup.separator = true;
        if(pi.cmd && pi.act)
            popup.add(pi.act);
    }
    
    if(!popup.triggered)
        popup.menu->addAction("No actions available");
    popup.menu->exec(QCursor::pos());
}

void            DreamMW::reconnect(QWidget* qw) 
{
    MainWindow::reconnect(qw);
    Window*w    = qobject_cast<Window*>(qw);
    if(w){
        connect(w, &Window::popupRequested, this, &DreamMW::popupRequested);
        connect(w, &Window::openRequested,  this, &DreamMW::openRequested);
    }
}

void    DreamMW::updateTitle()
{
    setWindowTitle(tr("YStudio -- %1").arg(qString(wksp::name())));
}


#include "moc_DreamMW.cpp"

