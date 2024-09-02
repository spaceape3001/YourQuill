////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <QApplication>
#include <gluon/app/Application.hpp>
#include <cstdlib>
#include <yq/text/match.hpp>
#include <yqResources.hpp>

#include "DoodleMW.hpp"

int main(int argc, char* argv[])
{
    using namespace yq;
    using namespace yq::gluon;
    using namespace yq::doodler;
    
    Application app(argc, argv);
    yq::resources::load_standard();
    
    QStringList     files;
    int             port    = 0;
        
    for(int i=1;i<argc;++i){
        if(is_similar(argv[i], "-p")){
            if(++i >= argc)
                break;
            port    = atoi(argv[i]);
        } else {
            files << argv[i];
        }
    }
    
    //  TODO deal with port (or push that into a full on editor)
    
    if(!files.empty()){
        size_t  cnt = 0;
        for(const QString& f : files){
            QProjectPtr qpp = QProject::load(f);
            if(!qpp.valid())
                continue;
            DoodleMW* mw  = new DoodleMW(qpp);
            mw -> show();
            ++cnt;
        }
        if(!cnt)
            return -1;
    } else {
        DoodleMW*   mw  = new DoodleMW;
        mw -> show();
    }
    return app.exec();
}
