#include <db/CommonDirs.hpp>
#include <yq/Logging.hpp>
#include <iostream>
#include <filesystem>

int main(int argc, char* argv[])
{
    log_to_std_error();
    CommonDir::init();
    std::cout << "Common Dir Info:\n"
              << "build: " << gDir.build << "\n"
              //<< "cache: " << gDir.cache << "\n"
              //<< "ipc:   " << gDir.ipc << "\n"
              //<< "ini:   " << gDir.ini << "\n"
              //<< "log:   " << gDir.log << "\n"
              //<< "pid:   " << gDir.pid << "\n"
              << "tmp:   " << gDir.tmp << "\n"
              << "share:\n";
    for(const auto&fs : gDir.share)
        std::cout << "  * " << fs << "\n";
    
    return 0;
}
