////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QUrl>
#include <QWebEngineUrlRequestInterceptor>
#include <QWebEngineView>
#include <QWidget>
#include <gui/main/SubWin.hpp>

class QLineEdit;

class WebBase : public SubWin {
    Q_OBJECT
public:
    WebBase(QWidget*parent=nullptr);
    ~WebBase();
    
    QUrl        home() const { return m_home; }
    void        setHome(const QUrl&);
    QString     title() const;
    
    
public slots:

    void        navigate(const QUrl&);
    void        submit(const QString&);
    void        goHome();
    void        refresh();

signals:
    void        appRequest(const QUrl&);
    void        titleChanged(const QString&);

private slots:
    void        urlChanged(const QUrl&);
    void        urlSubmit();

private:
    //class Network;
    class View;
    class Filter;
    
    QString     m_home;
    QUrl        m_lastUrl;
    QLineEdit*  m_url;
    View*       m_view;
};

class WebBase::Filter : public QWebEngineUrlRequestInterceptor {
    Q_OBJECT
public:
    Filter(WebBase*);
    void interceptRequest(QWebEngineUrlRequestInfo&) override;
private:
    WebBase*    m_web;
};

class WebBase::View : public QWebEngineView {
    Q_OBJECT
public:
    View();
    ~View();
};

