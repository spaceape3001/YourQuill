////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include <db/Workspace.hpp>
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
    Workspace::init(argv[1], Workspace::SEARCH);
    
    std::cout 
        << "Workspace Report:\n"
        << "-------------------\n"
        << "File     : " << gWksp.qfile << "\n"
        << "Key      : " << gWksp.qkey << "\n"
        << "Name     : " << gWksp.name << "\n"
        << "Author   : " << gWksp.author << "\n"
        << "Port     : " << gWksp.port << "\n"
        << "Templates: " << join(gWksp.templates, ", ") << "\n"
        << "Available: " << join(gWksp.available, ", ") << "\n"
        << "\n"
        << "Cache    : " << gWksp.cache << "\n"
        << "Hostname : " << gWksp.host << "\n"
        << "Logs     : " << gWksp.logs << "\n"
        << "Temp Dir : " << gWksp.tmp << "\n"
        << "\n"
        << "Roots:\n"
        << "-------------------\n"
    ;
    
    for(const Workspace::Root* rt : gWksp.roots)
        std::cout << rt->id << " [" << rt->key << "] " << rt->path << "\n";
    
    return 0;
}
