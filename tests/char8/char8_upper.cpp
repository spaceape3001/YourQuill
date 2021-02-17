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
    
    Char8   ch(argv[1][0]);
    if(argv[1][1] == '0'){
        return ch.is_upper() ? -1 : 0;
    } else if(argv[1][1] == '1'){
        return ch.is_upper() ? 0 : -1;
    } else {
        std::cerr << "Bad input!\n";
        return -1;
    }
    return -1;
}
