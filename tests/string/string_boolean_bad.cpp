////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "util/text/Strings.hpp"
#include <iostream>

using namespace yq;

int main(int argc, char* argv[])
{
    if(argc < 2){
        std::cerr << "Insufficient arguments!\n";
        return -1;
    }
    
    bool    ok  = String(argv[1]).to_boolean().good;
    return ok ? -1 : 0;
}
