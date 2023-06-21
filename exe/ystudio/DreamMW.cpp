////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "DreamMW.hpp"
#include "Browser.hpp"

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
#include <format>

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
    
    addAction("browser", "New Browser").connect(this, &DreamMW::newBrowser);
    addAction("refresh", "Refresh").icon(fetchIcon(":yq/icon/refresh%1.png")).shortcut("F5");

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
    
    makeMenu("studio", "Studio",
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
    makeMenu("view", "View",
        QStringList() << "refresh"
    );
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
    addWindow(new Browser(QString("http://localhost:%1/").arg(wksp::port())));
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

