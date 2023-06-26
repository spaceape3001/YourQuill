////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "DreamMW.hpp"
#include "DreamApp.hpp"
#include <gluon/core/Utilities.hpp>
#include <io/PluginLoader.hpp>
#include <meta/Meta.hpp>

using namespace yq;

//  MOTTO:  Detangling Photons

int main(int argc, char* argv[])
{
    QCoreApplication::setOrganizationName("YourQuill");

    DreamApp    app(argc, argv);
    if(!app.configure())
        return -1;

    Meta::init();
    load_plugin_dir("plugin");
    app.finalize();
    
    {
        DreamMW *   w   = new DreamMW;
        w -> newBrowser();
        w -> show();
    }
    
    return app.exec();
}

