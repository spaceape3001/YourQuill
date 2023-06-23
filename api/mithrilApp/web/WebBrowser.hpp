////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gluon/app/SubWindow.hpp>
#include <QUrl>
#include <QWebEnginePage>
#include <QWebEngineView>

class QLineEdit;

using namespace yq;

namespace yq::mithril {

    class WebBrowser : public yq::gluon::SubWindow {
        Q_OBJECT
    public:
    
        static void setHomeUrl(const QUrl&);
        static QUrl homeUrl();
    
        class View;
        class Page;
        
        View*       view() { return m_view; }
        const View* view() const { return m_view; }

    public:
        WebBrowser(const QUrl& location=QUrl(), QWebEngineProfile*profile=nullptr, QWidget* parent=nullptr);
        WebBrowser(QWebEngineProfile*profile, QWidget* parent=nullptr);
        ~WebBrowser();

    public slots:
        void    goHome();
        void    navigate(const QUrl&);
        
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
        static QUrl     s_homeUrl;
    };

    class WebBrowser::View : public QWebEngineView {
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

    class WebBrowser::Page : public QWebEnginePage {
        Q_OBJECT
    public:
        Page(QWebEngineProfile*profile, QObject*parent=nullptr);
        ~Page();
    };
}
