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
    if(argc<3){
        std::cerr << "Insufficent Arguments!\n";
        return -1;
    }
    
    return is_equal(argv[1], argv[2]) ? 0 : -1;
}
