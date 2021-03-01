////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "WebBase.hpp"
#include <dbgui/QuillApp.hpp>

#include <QHBoxLayout>
#include <QIcon>
#include <QLineEdit>
#include <QToolBar>
#include <QUrlQuery>
#include <QWebEnginePage>
#include <QVBoxLayout>

#include <math.h>

static constexpr const unsigned int   kStepsPerDouble         = 4;


WebBase::WebBase(QWidget*parent) : SubWin(parent), m_url{}, m_view{}
{
    m_url       = new QLineEdit;
    m_view      = new View;
    m_view->setContentsMargins(0,0,0,0);
    
    QWebEnginePage*   page    = m_view->page();
    page -> setUrlRequestInterceptor( new Filter(this));
    
    QToolBar*       bar = new QToolBar;
    
    bar -> addAction(QIcon(":icon/left.svg"), tr("Back"), m_view, &WebBase::View::back);
    bar -> addAction(QIcon(":icon/right.svg"), tr("Fwd"), m_view, &WebBase::View::forward);
    bar -> addAction(QIcon(":icon/refresh.svg"), tr("Reload"), m_view, &WebBase::View::reload);
    bar -> addAction(QIcon(":icon/home.svg"), tr("Home"), this, &WebBase::goHome);
    bar -> addWidget(m_url);
    
    QVBoxLayout* vlay = new QVBoxLayout(this);
    vlay -> addWidget(bar, 0);
    vlay -> addWidget(m_view, 1);
    vlay->setContentsMargins(0,0,0,0);


    connect(m_view, &WebBase::View::urlChanged, this, &WebBase::urlChanged);
    connect(m_url, &QLineEdit::returnPressed, this, &WebBase::urlSubmit);
    connect(m_view, &WebBase::View::titleChanged, this, &WebBase::titleChanged);
    //connect(m_view, &QWebView::linkClicked, this, &WebBase::navigate);
}

WebBase::~WebBase()
{
}

void        WebBase::goHome()
{
    submit(m_home);
}


void        WebBase::navigate(const QUrl& url)
{
    if(url.scheme() == "app"){
        emit appRequest(url);
        return;
    } 
    
    if(QuillApp::is_valid(url)){
        if(url == m_lastUrl)
            return ;
        m_view -> load(url);
    }
}

void        WebBase::refresh()
{
    m_view -> reload();
}


void        WebBase::submit(const QString& p)
{
    static const QString&   base    = QuillApp::url_base();
    navigate(QUrl(base + "/" + p));
}

QString     WebBase::title() const
{
    return m_view -> title();
}

void        WebBase::urlChanged(const QUrl& url)
{
    QString p   = url.path().mid(1);
    QString q   = url.query();
    if(!q.isEmpty())
        p       = p + "?" + q;
    m_url -> setText(p);
    m_lastUrl   = url;
}


void        WebBase::urlSubmit()
{
    submit(m_url -> text());
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

WebBase::Filter::Filter(WebBase* w) : QWebEngineUrlRequestInterceptor(w), m_web(w)
{
}

void WebBase::Filter::interceptRequest(QWebEngineUrlRequestInfo& info)
{
    QUrl    url = info.requestUrl();
    if(url.scheme().toLower() == "app"){
        m_web -> appRequest(url);
        info.block(true);
    } else 
        info.block(!QuillApp::is_valid(url));
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

WebBase::View::View()
{
}

WebBase::View::~View()
{
}

#include "moc_WebBase.cpp"

