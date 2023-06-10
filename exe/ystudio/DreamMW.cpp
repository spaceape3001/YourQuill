////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "DreamMW.hpp"
#include "Browser.hpp"
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
    
    makeMenu("studio", "Studio",
        QStringList() << "browser"
    );
    makeMenu("view", "View",
        QStringList() << "refresh"
    );
}

DreamMW::~DreamMW()
{
}

void    DreamMW::newBrowser()
{
    addWindow(new Browser(QString("http://localhost:%1/").arg(wksp::port())));
}

MainWindow*  DreamMW::newMain()
{
    return new DreamMW;
}

void    DreamMW::updateTitle()
{
    setWindowTitle(tr("YStudio -- %1").arg(qString(wksp::name())));
}


#include "moc_DreamMW.cpp"

