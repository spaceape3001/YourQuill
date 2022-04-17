////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
//  Utility for seeing how the workspace is being interpretted.
//
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include <db/root.hpp>
#include <db/workspace.hpp>

#include <yq/app/CmdArgs.hpp>
#include <yq/log/Logging.hpp>
#include <yq/meta/Meta.hpp>
#include <yq/text/text_utils.hpp>

int main(int argc, char* argv[])
{
    if(argc < 2){
        std::cerr << "Usage: wksp_read (workspace)\n";
        return -1;
    }
    
    using namespace yq;
    
    CmdArgs args(argc, argv);
    log_to_std_error();
    
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
