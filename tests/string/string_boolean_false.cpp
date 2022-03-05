////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "util/text/Utils.hpp"
#include <iostream>

using namespace yq;

int main(int argc, char* argv[])
{
    if(argc < 2){
        std::cerr << "Insufficient arguments!\n";
        return -1;
    }
    
    auto res    = to_boolean(argv[1]);
    return (res.good && !res.value) ? 0 : -1;
}
