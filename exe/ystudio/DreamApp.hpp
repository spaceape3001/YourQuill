////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gluon/app/Application.hpp>
#include <QUrl>

class DreamApp : public yq::gluon::Application {
    Q_OBJECT
public:
    static DreamApp*    app() { return s_app; }

    DreamApp(int&, char**);
    ~DreamApp();
    
    const QUrl&         home() const { return m_home; }
    
private:
    static DreamApp*    s_app;
    
    QUrl                m_home;
};

