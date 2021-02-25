////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "util/Logging.hpp"
#include "meta/Meta.hpp"
#include "util/Utilities.hpp"
#include "db/Workspace.hpp"
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
        << "Name: " << wksp::name() << "\n"
        << "Author: " << wksp::author() << "\n"
        << "Templates: " << join(wksp::templates(), ", ") << "\n"
        << "Available: " << join(wksp::templates_available(), ", ") << "\n"
    ;
    
    return 0;
}
