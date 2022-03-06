////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include <db/wksp/Root.hpp>
#include <db/wksp/Workspace.hpp>

#include <util/app/CmdArgs.hpp>
#include <util/log/Logging.hpp>
#include <util/meta/Meta.hpp>
#include <util/text/Utils.hpp>

int main(int argc, char* argv[])
{
    if(argc < 2){
        std::cerr << "Usage: wksp_read (workspace)\n";
        return -1;
    }
    
    using namespace yq;
    
    CmdArgs args(argc, argv);
    log_to_std_error();
    
    Meta::init();
    Meta::freeze();
    bool f = wksp::initialize(argv[1], wksp::SEARCH);
    
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
