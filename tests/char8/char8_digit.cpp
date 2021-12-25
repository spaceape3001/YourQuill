////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "util/text/Char8.hpp"
#include <iostream>

using namespace yq;

int main(int argc, char* argv[])
{
    if(argc < 2){
        std::cerr << "Insufficient arguments!\n";
        return -1;
    }
    
    Char8   ch(argv[1][0]);
    if(argv[1][1] == '0'){
        return ch.is_digit() ? -1 : 0;
    } else if(argv[1][1] == '1'){
        return ch.is_digit() ? 0 : -1;
    } else {
        std::cerr << "Bad input!\n";
        return -1;
    }
    return -1;
}
