////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <tbb/spin_rw_mutex.h>
#include <yq/gluon/app/Application.hpp>
#include <QUrl>

#ifdef YIDE_VULKAN
    #include <yq/tedit/app/TEApp.hpp>
    #include <yq/tachyon/app/AppCreateInfo.hpp>
    using DreamAppBase      = yq::tachyon::TEApp;
    using DreamAppConfig    = yq::tachyon::AppCreateInfo;
#else
    #include <yq/core/BasicApp.hpp>
    #include <yq/core/BasicAppConfig.hpp>
    using DreamAppBase      = yq::BasicApp;
    using DreamAppConfig    = yq::BasicAppConfig;
#endif

class DreamApp : public DreamAppBase {
public:

    static DreamApp*    app() { return s_app; }
    
    yq::gluon::Application*  qtApp() { return m_qApp; }

    DreamApp(int&, char**, const DreamAppConfig&);
    ~DreamApp();
    
    bool                start();
    
private:
    static DreamApp*    s_app;
    
    //QUrl                m_home;
    yq::gluon::Application*     m_qApp  = nullptr;
    
    int         m_argc      = 0;
    char**      m_argv      = nullptr;
};

