////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "DreamMW.hpp"
#include "DreamApp.hpp"

//  MOTTO:  Detangling Photons

int main(int argc, char* argv[])
{
    QCoreApplication::setOrganizationName("YourQuill");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    DreamApp    app(argc, argv);
    if(!app.configure())
        return -1;
    app.finalize();
    (new DreamMW) -> show();
    
    return app.exec();
}
