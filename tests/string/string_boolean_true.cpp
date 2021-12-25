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
    
    auto res    = String(argv[1]).to_boolean();
    return (res.good && res.value) ? 0 : -1;
}
