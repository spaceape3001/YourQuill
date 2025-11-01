////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "WebBrowser.hpp"

#include <yq/gluon/app/MainWindow.hpp>
#include <yq/gluon/core/Logging.hpp>
#include <yq/gluon/core/Utilities.hpp>
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
using namespace yq::mithril;
using namespace yq::gluon;

QUrl    WebBrowser::s_homeUrl;


////////////////////////////////////////////////////////////////////////////////
WebBrowser::Page::Page(QWebEngineProfile*profile, QObject*parent) : 
    QWebEnginePage(profile?profile:QWebEngineProfile::defaultProfile(), parent)
{
}

WebBrowser::Page::~Page()
{
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////

WebBrowser::View::View(Page* page, QWidget*parent) : QWebEngineView(parent), m_page(page)
{
    setPage(page);
    connect(this, &QWebEngineView::renderProcessTerminated, this, &WebBrowser::View::handle_renderProcessTerminated);
}

WebBrowser::View::~View()
{
}

QWebEngineView *WebBrowser::View::createWindow(QWebEnginePage::WebWindowType type) 
{
    MainWindow* mw  = MainWindow::mainWinFor(this);
    if(!mw)
        return nullptr;
    
    switch(type){
    case QWebEnginePage::WebBrowserWindow:
    case QWebEnginePage::WebBrowserTab:
        {
            WebBrowser*    b   = new WebBrowser;
            mw->addWindow(b);
            return b->view();
        }
        break;
    default:
        return nullptr;
    }
}

void        WebBrowser::View::handle_renderProcessTerminated(QWebEnginePage::RenderProcessTerminationStatus termStatus, int statusCode)
{
    switch(termStatus){
    case QWebEnginePage::NormalTerminationStatus:
        qtInfo << "WebBrowser::View: Render process normal exit";
        break;
    case QWebEnginePage::AbnormalTerminationStatus:
        qtInfo << "WebBrowser::View: Render process abnormal exit";
        break;
    case QWebEnginePage::CrashedTerminationStatus:
        qtInfo << "WebBrowser::View: Render process crashed";
        break;
    case QWebEnginePage::KilledTerminationStatus:
        qtInfo << "WebBrowser::View: Render process killed";
        break;        
    }
}


////////////////////////////////////////////////////////////////////////////////

QUrl WebBrowser::homeUrl()
{
    return s_homeUrl;
}

void WebBrowser::setHomeUrl(const QUrl&u)
{
    s_homeUrl   = u;
}

WebBrowser::WebBrowser(QWebEngineProfile*profile, QWidget* parent) : WebBrowser(QUrl(), profile, parent) 
{
}

WebBrowser::WebBrowser(const QUrl& location, QWebEngineProfile*profile, QWidget* parent) : Window(parent)
{
    m_page          = new Page(profile);
    m_view          = new View(m_page);
    
    m_url                   = new QLineEdit;
    connect(m_url, &QLineEdit::returnPressed, this, &WebBrowser::returnPressed);
    
    m_back          = new QAction(QIcon(":/icon/back.svg"), tr("Back"), this);
    connect(m_back, &QAction::triggered, m_view, &QWebEngineView::back);
    
    m_fore          = new QAction(QIcon(":/icon/fore.svg"), tr("Fore"), this);
    connect(m_fore,    &QAction::triggered, m_view, &QWebEngineView::forward);
    
    m_reload        = new QAction(QIcon(":/icon/refresh.svg"), tr("Reload"), this);
    connect(m_reload, &QAction::triggered, m_view, &QWebEngineView::reload);
    
    m_stop          = new QAction(QIcon(":/icon/stop.svg"), tr("Stop"), this);
    connect(m_stop, &QAction::triggered, m_view, &QWebEngineView::stop);
    
    m_home          = new QAction(QIcon(":/icon/home.svg"), tr("Home"), this);
    connect(m_home, &QAction::triggered, this, &WebBrowser::goHome);
    
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
    connect(m_view, &QWebEngineView::urlChanged, this, &WebBrowser::urlChanged);
    
//    connect(m_page, &WebBrowser::Viewer::actionRequested, this, &WebBrowser::actionRequested);

    QVBoxLayout*    vlay    = new QVBoxLayout;
    vlay->addWidget(bar);
    vlay->addWidget(m_view);
    vlay->setContentsMargins({0,0,0,0});
    setLayout(vlay);
    
    setWindowIcon(QIcon(":/icon/browser.svg"));
    
    navigate(location.isEmpty() ? s_homeUrl : location);
}

WebBrowser::~WebBrowser()
{
    m_page -> deleteLater();
}

void    WebBrowser::goHome()
{
    m_view->setUrl(homeUrl());
}

void    WebBrowser::navigate(const QUrl&u)
{
    if(!u.isEmpty())
        m_view->setUrl(u);
}

void    WebBrowser::refresh()
{
    m_view -> reload();
}

void    WebBrowser::returnPressed()
{
    QString     text    = m_url->text().trimmed();
    QUrl        newUrl  = QUrl::fromUserInput(text);
    m_view->setUrl(newUrl);
}

void    WebBrowser::urlChanged(const QUrl&u)
{
    m_url -> setText(u.toDisplayString());
}

#include "moc_WebBrowser.cpp"
