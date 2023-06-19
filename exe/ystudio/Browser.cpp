////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Browser.hpp"
#include "DreamApp.hpp"

//#include <basic/ErrorDB.hpp>
#include <gluon/app/MainWindow.hpp>
#include <gluon/core/Logging.hpp>
#include <gluon/core/Utilities.hpp>
#include <mithril/wksp/Workspace.hpp>

#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QHBoxLayout>
#include <QHostAddress>
#include <QLineEdit>
#include <QPushButton>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>

using namespace yq;
using namespace yq::gluon;

////////////////////////////////////////////////////////////////////////////////
Browser::Page::Page(QWebEngineProfile*profile, QObject*parent) : 
    QWebEnginePage(profile?profile:QWebEngineProfile::defaultProfile(), parent)
{
}

Browser::Page::~Page()
{
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////

Browser::View::View(Page* page, QWidget*parent) : QWebEngineView(parent), m_page(page)
{
    setPage(page);
    connect(this, &QWebEngineView::renderProcessTerminated, this, &Browser::View::handle_renderProcessTerminated);
}

Browser::View::~View()
{
}

QWebEngineView *Browser::View::createWindow(QWebEnginePage::WebWindowType type) 
{
    MainWindow* mw  = MainWindow::mainWinFor(this);
    if(!mw)
        return nullptr;
    
    switch(type){
    case QWebEnginePage::WebBrowserWindow:
    case QWebEnginePage::WebBrowserTab:
        {
            Browser*    b   = new Browser;
            mw->addWindow(b);
            return b->view();
        }
        break;
    default:
        return nullptr;
    }
}

void        Browser::View::handle_renderProcessTerminated(QWebEnginePage::RenderProcessTerminationStatus termStatus, int statusCode)
{
    switch(termStatus){
    case QWebEnginePage::NormalTerminationStatus:
        qtInfo << "Browser::View: Render process normal exit";
        break;
    case QWebEnginePage::AbnormalTerminationStatus:
        qtInfo << "Browser::View: Render process abnormal exit";
        break;
    case QWebEnginePage::CrashedTerminationStatus:
        qtInfo << "Browser::View: Render process crashed";
        break;
    case QWebEnginePage::KilledTerminationStatus:
        qtInfo << "Browser::View: Render process killed";
        break;        
    }
}


////////////////////////////////////////////////////////////////////////////////

Browser::Browser(QWebEngineProfile*profile, QWidget* parent) : Browser(QUrl(), profile, parent) 
{
}

Browser::Browser(const QUrl& location, QWebEngineProfile*profile, QWidget* parent) : SubWindow(parent)
{
    m_page          = new Page(profile);
    m_view          = new View(m_page);
    
    m_url                   = new QLineEdit;
    connect(m_url, &QLineEdit::returnPressed, this, &Browser::returnPressed);
    
    m_back          = new QAction(fetchIcon(":yq/icon/back%1.png"), tr("Back"), this);
    connect(m_back, &QAction::triggered, m_view, &QWebEngineView::back);
    
    m_fore          = new QAction(fetchIcon(":yq/icon/fore%1.png"), tr("Fore"), this);
    connect(m_fore,    &QAction::triggered, m_view, &QWebEngineView::forward);
    
    m_reload        = new QAction(fetchIcon(":yq/icon/refresh%1.png"), tr("Reload"), this);
    connect(m_reload, &QAction::triggered, m_view, &QWebEngineView::reload);
    
    m_stop          = new QAction(fetchIcon(":yq/icon/cancel%1.png"), tr("Stop"), this);
    connect(m_stop, &QAction::triggered, m_view, &QWebEngineView::stop);
    
    m_home          = new QAction(fetchIcon(":yq/icon/home%1.png"), tr("Home"), this);
    connect(m_home, &QAction::triggered, this, &Browser::goHome);
    
        //  Create the nav-bar
    
    QToolBar*           bar = new QToolBar;
    bar -> addAction(m_back);
    bar -> addAction(m_fore);
    bar -> addAction(m_home);
    bar -> addAction(m_reload);
    bar -> addAction(m_stop);
    bar -> addWidget(m_url);
    
    connect(m_view, &QWebEngineView::titleChanged, this, &QWidget::setWindowTitle);
    connect(m_view, &QWebEngineView::iconChanged, this, &QWidget::setWindowIcon);
    connect(m_view, &QWebEngineView::urlChanged, this, &Browser::urlChanged);
    
//    connect(m_page, &Browser::Viewer::actionRequested, this, &Browser::actionRequested);

    QVBoxLayout*    vlay    = new QVBoxLayout;
    vlay->addWidget(bar);
    vlay->addWidget(m_view);
    vlay->setContentsMargins({0,0,0,0});
    setLayout(vlay);
    
    if(!location.isEmpty())
        navigate(location);
}

Browser::~Browser()
{
    m_page -> deleteLater();
}

void    Browser::goHome()
{
    m_view->setUrl(DreamApp::app()->home());
}

void    Browser::navigate(const QUrl&u)
{
    if(!u.isEmpty())
        m_view->setUrl(u);
}

void    Browser::refresh()
{
    m_view -> reload();
}

void    Browser::returnPressed()
{
    QString     text    = m_url->text().trimmed();
    QUrl        newUrl  = QUrl::fromUserInput(text);
    m_view->setUrl(newUrl);
}

void    Browser::urlChanged(const QUrl&u)
{
    m_url -> setText(u.toDisplayString());
}



#include "moc_Browser.cpp"
