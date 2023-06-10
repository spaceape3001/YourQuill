////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gluon/app/SubWindow.hpp>
//#include <QUrl>
//#include <QTextBrowser>
//#include <basic/Url.hpp>
#include <QWebEnginePage>
#include <QWebEngineView>

class QLineEdit;

using namespace yq;

class Browser : public yq::gluon::SubWindow {
    Q_OBJECT
    
    class View;
    class Page;
    
    View*       view() { return m_view; }
    const View* view() const { return m_view; }

public slots:
    void    goHome();
    void    navigate(const QUrl&);
    
public:
    Browser(const QUrl& location=QUrl(), QWebEngineProfile*profile=nullptr, QWidget* parent=nullptr);
    Browser(QWebEngineProfile*profile, QWidget* parent=nullptr);
    ~Browser();

private slots:
    
    void    returnPressed();
    void    urlChanged(const QUrl&);
    void    refresh();
    
signals:
    void                actionRequested(const QUrl&);
    
private:
    View*           m_view          = nullptr;
    Page*           m_page          = nullptr;
    QLineEdit*      m_url           = nullptr;
    QAction*        m_back          = nullptr;
    QAction*        m_fore          = nullptr;
    QAction*        m_reload        = nullptr;
    QAction*        m_stop          = nullptr;
    QAction*        m_home          = nullptr;
};

class Browser::View : public QWebEngineView {
    Q_OBJECT
public:
    View(Page*, QWidget*parent=nullptr);
    ~View();

protected:
    QWebEngineView *createWindow(QWebEnginePage::WebWindowType type) override;

    void        handle_renderProcessTerminated(QWebEnginePage::RenderProcessTerminationStatus termStatus, int statusCode);
    
private:
    Page*       m_page  = nullptr;
    
};


class Browser::Page : public QWebEnginePage {
    Q_OBJECT
public:
    Page(QWebEngineProfile*profile, QObject*parent=nullptr);
    ~Page();
};

