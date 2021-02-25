#include "db/StdFile.hpp"
#include <iostream>
#include <filesystem>

int main(int argc, char* argv[])
{
    if(argc<3){
        std::cerr << "Usage: " << argv[0] << " (file-in) (file-out)\n";
        return -1;
    }
    
    std::filesystem::path  pin(argv[1]), pout(argv[2]);
    
    StdBodyFile         file;
    if(!file.load(pin)){
        std::cerr << "Unable to load\n";
        return -1;
    }
    
    if(!file.save_to(pout)){
        std::cerr << "Unable to save\n";
        return -1;
    }
    
    return 0;
}
