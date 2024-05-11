////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <QApplication>
#include <gluon/app/Application.hpp>
#include "DoodleMW.hpp"

int main(int argc, char* argv[])
{
    yq::gluon::Application app(argc, argv);
    DoodleMW*   mw  = new DoodleMW;
    mw -> show();
    return app.exec();
}
