////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Browser.hpp"
#include "DreamApp.hpp"

#include <basic/ErrorDB.hpp>
#include <gluon/app/MainWindow.hpp>
#include <gluon/core/Logging.hpp>
#include <gluon/core/Utilities.hpp>
#include <io/Curl.hpp>
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
Expect<Url>  Browser::toValidUrl(const QUrl& url)
{
    if(url.port() != yq::mithril::wksp::port())
        return unexpected<"Bad port">();
    QString    sch  = url.scheme().toLower();
    if((sch != "http") && (sch != "https"))
        return unexpected<"Bad scheme">();
    QString h   = url.host();
    if((h.toLower() != "localhost") && !QHostAddress(h).isLoopback())
        return unexpected<"Not localhost">();
    
    Url u;
    u.scheme    = url.scheme().toStdString();
    u.host      = url.host().toStdString();
    u.port      = yq::mithril::wksp::port();
    u.path      = url.path().toStdString();
    u.fragment  = url.fragment().toStdString();
    u.query     = url.query().toStdString();
    return u;
}


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



#if 0

Browser::Viewer::Viewer()
{
}

Browser::Viewer::~Viewer()
{
}

QVariant 	Browser::Viewer::loadResource(int type, const QUrl &url) 
{
    yInfo() << "Request to load resource(" << type << ", " << url << ")";

    if(url.scheme().toLower() == "yq"){
        //  this is action, nothing else....
        actionRequested(url);
        return QVariant();
    }
    
    auto x  = toValidUrl(url);
    if(!x){
        yInfo() << "Unable to load (" << url << "): " << x.error();
        return QVariant();
    }
    
    Curl    io;
    io.set_url(*x);
    HttpStatus  s   = io.exec();
    if(isError(s)){
        yInfo() << "HTTP (" << url << ") Error " << s.value() << " " << yq::statusMessage(s);
        return QVariant();
    }
    
    if(isRedirect(s)){
        yInfo() << "HTTP (" << url << ") Redirect " << s.value() << " " << yq::statusMessage(s) << " UNIMPLEMENTED";
        return QVariant();
    }
    
    auto data       = io.rx();
    return QByteArray((const char*) data.data(), data.size());
}

#endif


////////////////////////////////////////////////////////////////////////////////

Browser::Browser(QWebEngineProfile*profile, QWidget* parent) : Browser(QUrl(), profile, parent) 
{
}

Browser::Browser(const QUrl& location, QWebEngineProfile*profile, QWidget* parent) : SubWindow(parent)
{
    m_page          = new Page(profile, this);
    m_view          = new View(m_page);
    
    m_url                   = new QLineEdit;
    connect(m_url, &QLineEdit::returnPressed, this, &Browser::returnPressed);
    
    m_back          = new QAction(fetchIcon(":yq/icon/back%1.png"), tr("Back"), this);
    connect(m_back, &QAction::triggered, m_view, &QWebEngineView::back);
    
    m_fore          = new QAction(fetchIcon(":yq/icon/fore%1.png"), tr("Fore"), this);
    connect(m_fore,    &QAction::triggered, m_view, &QWebEngineView::forward);
    
    m_reload        = new QAction(fetchIcon(":yq/icon/reload%1.png"), tr("Reload"), this);
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
}

void    Browser::navigate(const QUrl&u)
{
    if(!u.isEmpty())
        m_view->setUrl(u);
}

void    Browser::returnPressed()
{
    QString     text    = m_url->text().trimmed();
    QUrl        newUrl  = QUrl::fromUserInput(text);
    m_view->setUrl(newUrl);
}

void    Browser::goHome()
{
    QUrl where(QString("http://localhost:%1/").arg(yq::mithril::wksp::port()));
    m_view->setUrl(where);
}

void    Browser::urlChanged(const QUrl&u)
{
    m_url -> setText(u.toDisplayString());
}



#include "moc_Browser.cpp"
