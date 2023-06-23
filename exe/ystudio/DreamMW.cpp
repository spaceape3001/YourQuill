////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "DreamMW.hpp"

#include <basic/Logging.hpp>
#include <gluon/core/Logging.hpp>

#include "Dock.hpp"
#include "WebBrowser.hpp"

#include <mithrilQt/atom/AtomTable.hpp>
#include <mithrilQt/book/BookTable.hpp>
#include <mithrilQt/category/CategoryTable.hpp>
#include <mithrilQt/character/CharacterTable.hpp>
#include <mithrilQt/class/ClassTable.hpp>
#include <mithrilQt/directory/DirectoryTable.hpp>
#include <mithrilQt/document/DocumentTable.hpp>
#include <mithrilQt/event/EventTable.hpp>
#include <mithrilQt/field/FieldTable.hpp>
#include <mithrilQt/folder/FolderTable.hpp>
#include <mithrilQt/fragment/FragmentTable.hpp>
#include <mithrilQt/game/GameTable.hpp>
#include <mithrilQt/leaf/LeafTable.hpp>
#include <mithrilQt/place/PlaceTable.hpp>
#include <mithrilQt/root/RootTable.hpp>
#include <mithrilQt/tag/TagList.hpp>
#include <mithrilQt/tag/TagTable.hpp>

#include <mithril/wksp/Workspace.hpp>
#include <gluon/core/Utilities.hpp>

#include <QMenu>

using namespace yq;
using namespace yq::gluon;
using namespace yq::mithril;


DreamMW::DreamMW()
{
    activateTabs();
    enableClosableTabs();
    enableDetachableTabs();
    enableAutoEnableCmds();
    
    resize(1920,1080);
    updateTitle();
    
    std::vector<QAction*>   dockActions;
    for(const DockInfo* di : DockInfo::all()){
        if(!di->isCreatable())
            continue;
        Object* obj = di->create();
        if(!obj)
            continue;
        
        Dock*   d   = static_cast<Dock*>(obj);
        
        QAction* act    = new QAction;
        d->m_action = act;

        act->setText(di->label());
        act->setIcon(di->icon());
        act->setShortcut(di->shortcut());
        act->setToolTip(di->toolTip());
        act->setCheckable(true);
        act->setChecked(di->isAutoStart());
        d->setVisible(di->isAutoStart());
        d->setAllowedAreas(di->allowedAreas());

        connect(act, &QAction::triggered, d, &Dock::triggered);
        addDock(di->startArea(), d);
        dockActions.push_back(act);
    }
    
    std::sort(dockActions.begin(), dockActions.end(), [](const QAction*a, const QAction*b)->bool {
        return QString::compare(a->text(), b->text(), Qt::CaseInsensitive) < 0;
    });
    
    addAction("browser", "New Browser").connect(this, &DreamMW::newBrowser);
    addAction("refresh", "Refresh").icon(QIcon(":/icon/refresh.svg")).shortcut("F5");

    addAction("atomTable", "Atoms").connect(this, &DreamMW::newAtomTable);
    addAction("bookTable", "Books").connect(this, &DreamMW::newBookTable);
    addAction("categoryTable", "Categories").connect(this, &DreamMW::newCategoryTable);
    addAction("characterTable", "Characters").connect(this, &DreamMW::newCharacterTable);
    addAction("classTable", "Classes").connect(this, &DreamMW::newClassTable);
    addAction("directoryTable", "Directories").connect(this, &DreamMW::newDirectoryTable);
    addAction("documentTable", "Documents").connect(this, &DreamMW::newDocumentTable);
    addAction("eventTable", "Events").connect(this, &DreamMW::newEventTable);
    addAction("fieldTable", "Fields").connect(this, &DreamMW::newFieldTable);
    addAction("folderTable", "Folders").connect(this, &DreamMW::newFolderTable);
    addAction("fragmentTable", "Fragments").connect(this, &DreamMW::newFragmentTable);
    addAction("gameTable", "Games").connect(this, &DreamMW::newGameTable);
    addAction("leafTable", "Leafs").connect(this, &DreamMW::newLeafTable);
    addAction("placeTable", "Places").connect(this, &DreamMW::newPlaceTable);
    addAction("rootTable", "Roots").connect(this, &DreamMW::newRootTable);
    addAction("tagTable", "Tags").connect(this, &DreamMW::newTagTable);
    addAction("tagList", "Tag List").connect(this, &DreamMW::newTagList);
    
    QMenu* fileMenu     = makeMenu("file", "File");
    QMenu* editMenu     = makeMenu("edit", "Edit");
    QMenu* viewMenu     = makeMenu("view", "View");
    QMenu* studioMenu   = makeMenu("studio", "Studio");
    QMenu* windowMenu   = makeMenu("window", "Window");
    QMenu* helpMenu     = makeMenu("help", "Help");
    QMenu* debugMenu    = makeMenu("debug", "Debug");

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
            << "--"
            << "refresh"
    );
    
    addToMenu(studioMenu,
        QStringList() 
            << "browser" 
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

void    DreamMW::newAtomTable()
{
    addWindow(new AtomTable(ALL));
}

void    DreamMW::newBookTable()
{
    addWindow(new BookTable(ALL));
}

void    DreamMW::newBrowser()
{
    addWindow(new WebBrowser);
}

void    DreamMW::newCategoryTable()
{
    addWindow(new CategoryTable(ALL));
}

void    DreamMW::newCharacterTable()
{
    addWindow(new CharacterTable(ALL));
}

void    DreamMW::newClassTable()
{
    addWindow(new ClassTable(ALL));
}

void    DreamMW::newDirectoryTable()
{
    addWindow(new DirectoryTable(ALL));
}

void    DreamMW::newDocumentTable()
{
    addWindow(new DocumentTable(ALL));
}

void    DreamMW::newEventTable()
{
    addWindow(new EventTable(ALL));
}

void    DreamMW::newFieldTable()
{
    addWindow(new FieldTable(ALL));
}

void    DreamMW::newFolderTable()
{
    addWindow(new FolderTable(ALL));
}

void    DreamMW::newFragmentTable()
{
    addWindow(new FragmentTable(ALL));
}

void    DreamMW::newGameTable()
{
    addWindow(new GameTable(ALL));
}

void    DreamMW::newLeafTable()
{
    addWindow(new LeafTable(ALL));
}

MainWindow*  DreamMW::newMain()
{
    return new DreamMW;
}

void    DreamMW::newPlaceTable()
{
    addWindow(new PlaceTable(ALL));
}

void    DreamMW::newRootTable()
{
    addWindow(new RootTable(ALL));
}

void    DreamMW::newTagList()
{
    addWindow(new TagList(ALL));
}

void    DreamMW::newTagTable()
{
    addWindow(new TagTable(ALL));
}

void    DreamMW::updateTitle()
{
    setWindowTitle(tr("YStudio -- %1").arg(qString(wksp::name())));
}


#include "moc_DreamMW.cpp"

