////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include <db/Root.hpp>
#include <db/Wksp.hpp>
#include <meta/Meta.hpp>
#include <util/Logging.hpp>
#include <util/Utilities.hpp>

#include <QCoreApplication>

int main(int argc, char* argv[])
{
    if(argc < 2){
        std::cerr << "Usage: wksp_read (workspace)\n";
        return -1;
    }

    log_to_std_error();
    QCoreApplication    app(argc, argv);
    meta_init();
    meta_freeze();
    wksp::initialize(argv[1], wksp::Option::SEARCH);
    
    std::cout 
        << "Workspace Report:\n"
        << "-------------------\n"
        << "Name     : " << wksp::name() << "\n"
        << "Author   : " << wksp::author() << "\n"
        << "Templates: " << join(wksp::templates(), ", ") << "\n"
        << "Available: " << join(wksp::templates_available(), ", ") << "\n"
        << "\n"
        << "Cache    : " << wksp::cache_db() << "\n"
        << "Hostname : " << wksp::hostname() << "\n"
        << "Logs     : " << wksp::log_dir_path() << "\n"
        << "Temp Dir : " << wksp::temp_dir_path() << "\n"
        << "\n"
        << "Roots:\n"
        << "-------------------\n"
    ;
    
    for(const Root* rt : wksp::roots())
        std::cout << rt->id() << " [" << rt->key() << "] " << rt->path() << "\n";
    
    return 0;
}
