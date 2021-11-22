////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include <db/Root.hpp>
#include <db/Wksp.hpp>
#include <meta/Meta.hpp>
#include <util/CmdArgs.hpp>
#include <util/Logging.hpp>
#include <util/Utilities.hpp>

int main(int argc, char* argv[])
{
    if(argc < 2){
        std::cerr << "Usage: wksp_read (workspace)\n";
        return -1;
    }
    
    CmdArgs args(argc, argv);
    log_to_std_error();
    
    meta_init();
    meta_freeze();
    wksp::initialize(argv[1], wksp::SEARCH);
    
    std::cout 
        << "Workspace Report:\n"
        << "-------------------\n"
        << "Directory: " << wksp::quill_dir() << "\n"
        << "File     : " << wksp::quill_file() << "\n"
        << "Key      : " << wksp::quill_key() << "\n"
        << "Name     : " << wksp::name() << "\n"
        << "Author   : " << wksp::author() << "\n"
        << "Port     : " << wksp::port() << "\n"
        << "Templates: " << join(wksp::templates(), ", ") << "\n"
        << "Available: " << join(wksp::templates_available(), ", ") << "\n"
        << "\n"
        << "Cache    : " << wksp::cache_db() << "\n"
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
