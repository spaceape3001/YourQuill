////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
//  Utility for seeing how the workspace is being interpretted.
//
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include <basic/BasicApp.hpp>
#include <basic/Logging.hpp>
#include <basic/meta/Meta.hpp>
#include <basic/TextUtils.hpp>

#include <kernel/Root.hpp>
#include <kernel/Workspace.hpp>

int main(int argc, char* argv[])
{
    if(argc < 2){
        std::cerr << "Usage: wksp_read (workspace)\n";
        return -1;
    }
    
    using namespace yq;
    
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
    
    for(const Root* rt : wksp::roots())
        std::cout << rt->id << " [" << rt->key << "] " << rt->path << "\n";
    
    return 0;
}
