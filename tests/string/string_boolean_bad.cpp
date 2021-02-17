////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "yq/util/String.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    if(argc < 2){
        std::cerr << "Insufficient arguments!\n";
        return -1;
    }
    
    bool    ok  = String(argv[1]).to_boolean().good;
    return ok ? -1 : 0;
}
