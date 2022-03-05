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
    
    char    ch(argv[1][0]);
    if(argv[1][1] == '0'){
        return is_alnum(ch) ? -1 : 0;
    } else if(argv[1][1] == '1'){
        return is_alnum(ch) ? 0 : -1;
    } else {
        std::cerr << "Bad input!\n";
        return -1;
    }
    return -1;
}
