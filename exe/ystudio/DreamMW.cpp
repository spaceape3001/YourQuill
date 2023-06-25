////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "DreamMW.hpp"

#include <basic/Logging.hpp>
#include <basic/Map.hpp>
#include <basic/Vector.hpp>
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
    for(const auto& di : Dock::all()){
    
        Dock*   d   = di.fnCreate();
        
        ActionInfo  ai  = di.action;
        ai.checkable    = true;
        
        QAction* act    = createAction(ai);
        d->m_action = act;
        d->setVisible(ai.checked);
        d->setAllowedAreas(di.allowedAreas);
        d->setWindowTitle(ai.label);

        connect(act, &QAction::triggered, d, &Dock::triggered);
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
    addAction("refresh", "Refresh").icon(QIcon(":/icon/refresh.svg")).shortcut("F5");
    
    m_actView   = new QAction("View", this);
    connect(m_actView, &QAction::triggered, this, &DreamMW::cmd_view);
    
    m_actEdit   = new QAction("Edit", this);
    connect(m_actEdit, &QAction::triggered, this, &DreamMW::cmd_edit);
    
    m_actDelete = new QAction("Delete", this);
    connect(m_actDelete, &QAction::triggered, this, &DreamMW::cmd_delete);
    
    m_actProperties = new QAction("Properties", this);
    connect(m_actProperties, &QAction::triggered, this, &DreamMW::cmd_properties);

    m_popupMenu     = new QMenu;
    
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
    );

    addToMenu(editMenu,
        QStringList()
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


    addToMenu(viewMenu,
        QStringList()
            << "--"
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
    m_popupMenu -> deleteLater();
}

QDockWidget*    DreamMW::addDock(Qt::DockWidgetArea dwa, QWidget*q)
{
    QDockWidget* dw = MainWindow::addDock(dwa, q);
    
    Dock*   d   = qobject_cast<Dock*>(q);
    if(d){
        connect(d, &Dock::popupRequested, this, &DreamMW::popupRequested);
        connect(d, &Dock::openRequested,  this, &DreamMW::openRequested);
    }
    return dw;
}

void    DreamMW::cmd_delete()
{
    if(m_cmdDelete && m_idForCmd)
        m_cmdDelete -> invoke(this, m_idForCmd);
}

void    DreamMW::cmd_edit()
{
    if(m_cmdEdit && m_idForCmd)
        m_cmdEdit -> invoke(this, m_idForCmd);
}

void    DreamMW::cmd_properties()
{
    if(m_cmdProperties && m_idForCmd)
        m_cmdProperties -> invoke(this, m_idForCmd);
}

void    DreamMW::cmd_view()
{
    if(m_cmdView && m_idForCmd)
        m_cmdView -> invoke(this, m_idForCmd);
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
    for(const Command* cmd : Command::all()){
        if(cmd -> flavor() != ArgFlavor::Id)    // command's not ID triggered... cycle
            continue;
        if(cmd -> type() != Command::Type::View)
            continue;
        if(!cmd -> accept(i))                   // give the command a chance to reject
            continue;
        cmd -> invoke(this, i);
        break;
    }
}

void    DreamMW::popupRequested(Id i)
{
    /*
        A popup has been requested by the user (in wherever).  
        As this menu's dynamic, we'll be forming it dynamically.
    */

    // Set the ID for all subsequent actions
    m_idForCmd      = i;
    
    // Reset the view/edit/delete commands
    m_cmdView       = m_cmdEdit = m_cmdDelete = m_cmdProperties = nullptr;

    // Clear the old menu
    m_popupMenu -> clear();
    
    // Need to know the actions to populate (between view/edit & delete)
    std::vector<QAction*>   actions;
    
    for(const Command* cmd : commands()){
        if(cmd -> flavor() != ArgFlavor::Id)    // command's not ID triggered... cycle
            continue;
        if(!cmd -> accept(i))                   // give the command a chance to reject
            continue;
            
        switch(cmd -> type()){                  // search for view/edit/delete
        case Command::Type::View:
            if(!m_cmdView){
                m_cmdView   = cmd;
                m_actView->setText(QString("%2 %1").arg(qString(i.name())).arg(cmd->label()));
                continue;
            }
            break;
        case Command::Type::Edit:
            if(!m_cmdEdit){
                m_cmdEdit   = cmd;
                m_actEdit->setText(QString("%2 %1").arg(qString(i.name())).arg(cmd->label()));
                continue;
            }
            break;
        case Command::Type::Delete:
            if(!m_cmdDelete){
                m_cmdDelete = cmd;
                m_actDelete->setText(QString("%2 %1").arg(qString(i.name())).arg(cmd->label()));
                continue;
            }
            break;
        case Command::Type::Properties:
            if(!m_cmdProperties){
                m_cmdProperties = cmd;
                m_actProperties->setText(QString("%2 %1").arg(qString(i.name())).arg(cmd->label()));
                continue;
            }
            break;
        default:
            break;
        }
        
        //  if it reaches here... it'll go on the list
        actions.push_back(m_cmdActions[cmd->id()]);
    }
    
    //  Now we'll add the items, start with view & edit
    if(m_cmdView || m_cmdEdit){
        if(m_cmdView)
            m_popupMenu -> addAction(m_actView);
        if(m_cmdEdit)
            m_popupMenu -> addAction(m_actEdit);
        
        if(!actions.empty() || m_cmdDelete || m_cmdProperties)
            m_popupMenu -> addSeparator();
    }
    
    //  Add the non-conforming options
    if(!actions.empty()){
        for(QAction* a : actions)
            m_popupMenu -> addAction(a);
        if(m_cmdDelete || m_cmdProperties)
            m_popupMenu -> addSeparator();
    }
    
    //  Delete goes last
    if(m_cmdDelete){
        m_popupMenu -> addAction(m_actDelete);
        if(m_cmdProperties)
            m_popupMenu -> addSeparator();
    }
    
    if(m_cmdProperties)
        m_popupMenu -> addAction(m_actProperties);
    
    if(m_popupMenu->actions().isEmpty())
        m_popupMenu->addAction("No actions available");
    
    m_popupMenu->exec(QCursor::pos());
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


#include "Action.ipp"
#include "Command.ipp"
#include "Dock.ipp"
#include "Window.ipp"


#include "moc_DreamMW.cpp"
#include "moc_Dock.cpp"
#include "moc_Window.cpp"

