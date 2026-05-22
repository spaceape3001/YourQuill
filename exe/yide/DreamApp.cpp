////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "common.hpp"
#include "DreamApp.hpp"

#include <QWebEngineSettings>

#include "WebBrowser.hpp"

#include <yq/net/Url.hpp>
#include <yq/gluon/app/Application.hpp>
#include <yq/gluon/core/Logging.hpp>
#include <yq/net/Curl.hpp>
#include <yq/process/PluginLoader.hpp>
#include <yq/mithril/wksp/Workspace.hpp>
#include <yq/sql/SqlLite.hpp>
#include <yq/net/json.hpp>
#include <QFont>

using namespace yq;
using namespace yq::gluon;
using namespace yq::mithril;


DreamApp*    DreamApp::s_app    = nullptr;

DreamApp::DreamApp(int& argc, char**argv, const DreamAppConfig& aci) : DreamAppBase(argc, argv, aci), m_argc(argc), m_argv(argv)
{
    if(!s_app)
        s_app   = this;
        
    //QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::PluginsEnabled, false);
    //QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::DnsPrefetchEnabled, false);
    //QWebEngineProfile::defaultProfile()->setUseForGlobalCertificateVerification();
}

DreamApp::~DreamApp()
{
    #ifndef YIDE_VULKAN
    delete m_qApp;
    #endif
    m_qApp  = nullptr;

    if(s_app == this)
        s_app = nullptr;
}

bool    DreamApp::start()
{
    #ifdef YIDE_VULKAN
    if(!DreamAppBase::start())
        return false;
    m_qApp  = dynamic_cast<gluon::Application*>(app_thread());
    #else 
    m_qApp  = new gluon::Application(m_argc, m_argv);
    #endif

    //  HACK to increase font size... change this to a config type of file
    //  Try a simple 50% scaling
    QFont   f   = QApplication::font();
    f.setPointSize((f.pointSize() * 3) >> 1);
    QApplication::setFont(f);

    return true;
}



