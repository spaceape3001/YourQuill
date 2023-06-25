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
#include <gluon/util/UInt64SignalMapper.hpp>

#include "Action.hpp"
#include "Command.hpp"
#include "Dock.hpp"
#include "WebBrowser.hpp"
#include "Window.hpp"

//#include <mithrilQt/atom/AtomTable.hpp>
//#include <mithrilQt/book/BookTable.hpp>
//#include <mithrilQt/category/CategoryTable.hpp>
//#include <mithrilQt/character/CharacterTable.hpp>
//#include <mithrilQt/class/ClassTable.hpp>
//#include <mithrilQt/directory/DirectoryTable.hpp>
//#include <mithrilQt/document/DocumentTable.hpp>
//#include <mithrilQt/event/EventTable.hpp>
//#include <mithrilQt/field/FieldTable.hpp>
//#include <mithrilQt/folder/FolderTable.hpp>
//#include <mithrilQt/fragment/FragmentTable.hpp>
//#include <mithrilQt/game/GameTable.hpp>
//#include <mithrilQt/leaf/LeafTable.hpp>
//#include <mithrilQt/place/PlaceTable.hpp>
//#include <mithrilQt/root/RootTable.hpp>
//#include <mithrilQt/tag/TagList.hpp>
//#include <mithrilQt/tag/TagTable.hpp>

#include <mithril/wksp/Workspace.hpp>
#include <gluon/core/Utilities.hpp>

#include <QMenu>
#include <QMenuBar>

using namespace yq;
using namespace yq::gluon;
using namespace yq::mithril;

struct KCmdData {
    const Command*  cmd = nullptr;
    QAction*        act = nullptr;
};



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
    
    
    UInt64SignalMapper*     cmdMapper   = new UInt64SignalMapper(this);
    connect(cmdMapper, &UInt64SignalMapper::mapped, this, &DreamMW::commandTriggered);
    
    std::vector<const Command*>   commands;
    for(const Command* cmd : Command::all()){
        if(!cmd)
            continue;
        commands.push_back(cmd);
    }
    
    std::sort(commands.begin(), commands.end(), [](const Command* a, const Command* b) -> bool {
        if(a->gravity() < b->gravity())
            return true;
        if(a->gravity() > b->gravity())
            return false;
        return QString::compare(a->label(), b->label(), Qt::CaseInsensitive) < 0;
    });
            
    for(const Command *cmd : commands){
        QAction*    act = createAction(cmd->action());
        connect(act, &QAction::triggered, cmdMapper, &UInt64SignalMapper::map);
        cmdMapper->setMapping(act, cmd->id());
        m_cmdActions[cmd->id()]     = act;
    }
    
    addAction("browser", "New Browser").connect(this, &DreamMW::newBrowser);
    addAction("refresh", "Refresh").icon(QIcon(":/icon/refresh.svg")).shortcut("F5");

    //addAction("atomTable", "Atoms").connect(this, &DreamMW::newAtomTable);
    //addAction("bookTable", "Books").connect(this, &DreamMW::newBookTable);
    //addAction("categoryTable", "Categories").connect(this, &DreamMW::newCategoryTable);
    //addAction("characterTable", "Characters").connect(this, &DreamMW::newCharacterTable);
    //addAction("classTable", "Classes").connect(this, &DreamMW::newClassTable);
    //addAction("directoryTable", "Directories").connect(this, &DreamMW::newDirectoryTable);
    //addAction("documentTable", "Documents").connect(this, &DreamMW::newDocumentTable);
    //addAction("eventTable", "Events").connect(this, &DreamMW::newEventTable);
    //addAction("fieldTable", "Fields").connect(this, &DreamMW::newFieldTable);
    //addAction("folderTable", "Folders").connect(this, &DreamMW::newFolderTable);
    //addAction("fragmentTable", "Fragments").connect(this, &DreamMW::newFragmentTable);
    //addAction("gameTable", "Games").connect(this, &DreamMW::newGameTable);
    //addAction("leafTable", "Leafs").connect(this, &DreamMW::newLeafTable);
    //addAction("placeTable", "Places").connect(this, &DreamMW::newPlaceTable);
    //addAction("rootTable", "Roots").connect(this, &DreamMW::newRootTable);
    //addAction("tagTable", "Tags").connect(this, &DreamMW::newTagTable);
    //addAction("tagList", "Tag List").connect(this, &DreamMW::newTagList);
    
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
    for(const Command* cmd : commands){
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
    
    
    #if 0
            << "--" 
            << "atomTable" 
            << "bookTable"
            << "categoryTable"
            << "characterTable"
            << "classTable"
            << "directoryTable"
            << "documentTable"
            << "eventTable"
            << "fieldTable"
            << "folderTable"
            << "fragmentTable"
            << "gameTable"
            << "leafTable"
            << "placeTable"
            << "rootTable"
            << "tagTable"
            << "--"
            << "tagList"
    );
    #endif
    
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
    
    Dock*   d   = qobject_cast<Dock*>(q);
    if(d){
        connect(d, &Dock::popupRequested, this, &DreamMW::popupRequested);
        connect(d, &Dock::openRequested,  this, &DreamMW::openRequested);
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

//void    DreamMW::newAtomTable()
//{
    //addWindow(new AtomTable(ALL));
//}

//void    DreamMW::newBookTable()
//{
    //addWindow(new BookTable(ALL));
//}

void    DreamMW::newBrowser()
{
    addWindow(new WebBrowser);
}

//void    DreamMW::newCategoryTable()
//{
    //addWindow(new CategoryTable(ALL));
//}

//void    DreamMW::newCharacterTable()
//{
    //addWindow(new CharacterTable(ALL));
//}

//void    DreamMW::newClassTable()
//{
    //addWindow(new ClassTable(ALL));
//}

//void    DreamMW::newDirectoryTable()
//{
    //addWindow(new DirectoryTable(ALL));
//}

//void    DreamMW::newDocumentTable()
//{
    //addWindow(new DocumentTable(ALL));
//}

//void    DreamMW::newEventTable()
//{
    //addWindow(new EventTable(ALL));
//}

//void    DreamMW::newFieldTable()
//{
    //addWindow(new FieldTable(ALL));
//}

//void    DreamMW::newFolderTable()
//{
    //addWindow(new FolderTable(ALL));
//}

//void    DreamMW::newFragmentTable()
//{
    //addWindow(new FragmentTable(ALL));
//}

//void    DreamMW::newGameTable()
//{
    //addWindow(new GameTable(ALL));
//}

//void    DreamMW::newLeafTable()
//{
    //addWindow(new LeafTable(ALL));
//}

MainWindow*  DreamMW::newMain()
{
    return new DreamMW;
}

//void    DreamMW::newPlaceTable()
//{
    //addWindow(new PlaceTable(ALL));
//}

//void    DreamMW::newRootTable()
//{
    //addWindow(new RootTable(ALL));
//}

//void    DreamMW::newTagList()
//{
    //addWindow(new TagList(ALL));
//}

//void    DreamMW::newTagTable()
//{
    //addWindow(new TagTable(ALL));
//}

void    DreamMW::updateTitle()
{
    setWindowTitle(tr("YStudio -- %1").arg(qString(wksp::name())));
}

void    DreamMW::popupRequested(Id i)
{
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

void    DreamMW::openRequested(Id i)
{
}


#include "Action.ipp"
#include "Command.ipp"
#include "Dock.ipp"
#include "Window.ipp"


#include "moc_DreamMW.cpp"
#include "moc_Dock.cpp"
#include "moc_Window.cpp"

