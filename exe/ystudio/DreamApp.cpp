////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "DreamApp.hpp"

#include <QWebEngineSettings>

namespace yq {
    namespace resources {
        void    initialize();
    }
}

DreamApp*    DreamApp::s_app    = nullptr;

DreamApp::DreamApp(int& argc, char**argv) : yq::gluon::Application(argc, argv)
{
    yq::resources::initialize();
    
    if(!s_app)
        s_app   = this;
        
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::PluginsEnabled, false);
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::DnsPrefetchEnabled, false);
    //QWebEngineProfile::defaultProfile()->setUseForGlobalCertificateVerification();
}

DreamApp::~DreamApp()
{
    if(s_app == this)
        s_app = nullptr;
}

#include "moc_DreamApp.cpp"
