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
#include "table/EventTable.hpp"
#include "table/GameTable.hpp"
#include "table/PlaceTable.hpp"

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
    addAction("eventTable", "Events").connect(this, &DreamMW::newBookTable);
    addAction("placeTable", "Places").connect(this, &DreamMW::newBookTable);
    
    makeMenu("studio", "Studio",
        QStringList() 
            << "browser" 
            << "--" 
            << "atomTable" 
            << "bookTable"
            << "characterTable"
            << "eventTable"
            << "placeTable"
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

void    DreamMW::newEventTable()
{
    EventTable*  tt  = new EventTable;
    tt->refresh();
    addWindow(tt);
}

void    DreamMW::newGameTable()
{
    GameTable*  tt  = new GameTable;
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

void    DreamMW::updateTitle()
{
    setWindowTitle(tr("YStudio -- %1").arg(qString(wksp::name())));
}


#include "moc_DreamMW.cpp"

