////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
//  Utility for seeing how the workspace is being interpretted.
//
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include <0/basic/BasicApp.hpp>
#include <yq/basic/Logging.hpp>
#include <0/meta/Meta.hpp>
#include <yq/text/join.hpp>


#include <mithril/root/RootDir.hpp>
#include <mithril/wksp/Workspace.hpp>

using namespace yq;
using namespace yq::mithril;

int main(int argc, char* argv[])
{
    if(argc < 2){
        std::cerr << "Usage: wksp_read (workspace)\n";
        return -1;
    }
    
    BasicApp args(argc, argv);
    
    wksp::Config    cfg;
    cfg.spec = argv[1];
    cfg.options = wksp::SEARCH;
    Meta::init();
    bool f = wksp::initialize(cfg);
    Meta::freeze();
    
    
    
    
    if(!f){
        std::cerr << "INITIALZATION FAILED!\n";
        return -1;
    }
    
    std::cout 
        << "Workspace Report:\n"
        << "-------------------\n"
        << "Directory: " << wksp::quill_dir() << "\n"
        << "File     : " << wksp::quill_file() << "\n"
        << "Key      : " << wksp::quill_key() << "\n"
        << "Name     : " << wksp::name() << "\n"
        << "Author   : " << wksp::author() << "\n"
        << "Port     : " << wksp::port() << "\n"
        << "Templates: " << join(wksp::templates(false), ", ") << "\n"
        << "Available: " << join(wksp::templates(true), ", ") << "\n"
        << "\n"
        << "Cache    : " << wksp::cache() << "\n"
        << "Hostname : " << wksp::host() << "\n"
        << "Logs     : " << wksp::log_dir() << "\n"
        << "Temp Dir : " << wksp::temp_dir() << "\n"
        << "\n"
        << "Roots:\n"
        << "-------------------\n"
    ;
    
    for(const RootDir* rt : wksp::root_dirs())
        std::cout << rt->id << " [" << rt->key << "] " << rt->path << "\n";
    
    return 0;
}
