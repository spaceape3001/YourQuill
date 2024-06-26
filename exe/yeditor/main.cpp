////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "DreamMW.hpp"
#include "DreamApp.hpp"
#include <gluon/core/Utilities.hpp>
#include <0/io/PluginLoader.hpp>
#include <0/meta/Meta.hpp>
#include <yqResources.hpp>


//  MOTTO:  Detangling Photons

int main(int argc, char* argv[])
{
    using namespace yq;
    
    QCoreApplication::setOrganizationName("YourQuill");

    DreamApp    app(argc, argv);
    if(!app.configure())
        return -1;
    yq::resources::load_standard();

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

