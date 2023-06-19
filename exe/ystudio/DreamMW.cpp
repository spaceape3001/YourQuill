////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "DreamMW.hpp"
#include "Browser.hpp"

#include "table/AtomTable.hpp"
#include "table/BookTable.hpp"
#include "table/CharacterTable.hpp"
#include "table/ClassTable.hpp"
#include "table/DirectoryTable.hpp"
#include "table/DocumentTable.hpp"
#include "table/EventTable.hpp"
#include "table/FieldTable.hpp"
#include "table/FolderTable.hpp"
#include "table/FragmentTable.hpp"
#include "table/GameTable.hpp"
#include "table/LeafTable.hpp"
#include "table/PlaceTable.hpp"
#include "table/RootTable.hpp"
#include "table/TagTable.hpp"

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
    
    makeMenu("studio", "Studio",
        QStringList() 
            << "browser" 
            << "--" 
            << "atomTable" 
            << "bookTable"
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
    AtomTable*  tt  = new AtomTable;
    tt->refresh();
    addWindow(tt);
}


void    DreamMW::newBookTable()
{
    BookTable*  tt  = new BookTable;
    tt->refresh();
    addWindow(tt);
}

void    DreamMW::newBrowser()
{
    addWindow(new Browser(QString("http://localhost:%1/").arg(wksp::port())));
}

void    DreamMW::newCharacterTable()
{
    CharacterTable*  tt  = new CharacterTable;
    tt->refresh();
    addWindow(tt);
}

void    DreamMW::newClassTable()
{
    ClassTable*  tt  = new ClassTable;
    tt->refresh();
    addWindow(tt);
}

void    DreamMW::newDirectoryTable()
{
    DirectoryTable*  tt  = new DirectoryTable;
    tt->refresh();
    addWindow(tt);
}

void    DreamMW::newDocumentTable()
{
    DocumentTable*  tt  = new DocumentTable;
    tt->refresh();
    addWindow(tt);
}

void    DreamMW::newEventTable()
{
    EventTable*  tt  = new EventTable;
    tt->refresh();
    addWindow(tt);
}

void    DreamMW::newFieldTable()
{
    FieldTable*  tt  = new FieldTable;
    tt->refresh();
    addWindow(tt);
}

void    DreamMW::newFolderTable()
{
    FolderTable*  tt  = new FolderTable;
    tt->refresh();
    addWindow(tt);
}

void    DreamMW::newFragmentTable()
{
    FragmentTable*  tt  = new FragmentTable;
    tt->refresh();
    addWindow(tt);
}

void    DreamMW::newGameTable()
{
    GameTable*  tt  = new GameTable;
    tt->refresh();
    addWindow(tt);
}

void    DreamMW::newLeafTable()
{
    LeafTable*  tt  = new LeafTable;
    tt->refresh();
    addWindow(tt);
}

MainWindow*  DreamMW::newMain()
{
    return new DreamMW;
}

void    DreamMW::newPlaceTable()
{
    PlaceTable*  tt  = new PlaceTable;
    tt->refresh();
    addWindow(tt);
}

void    DreamMW::newRootTable()
{
    RootTable*  tt  = new RootTable;
    tt->refresh();
    addWindow(tt);
}

void    DreamMW::newTagTable()
{
    TagTable*  tt  = new TagTable;
    tt->refresh();
    addWindow(tt);
}

void    DreamMW::updateTitle()
{
    setWindowTitle(tr("YStudio -- %1").arg(qString(wksp::name())));
}


#include "moc_DreamMW.cpp"

