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
    if(argc<3){
        std::cerr << "Insufficent Arguments!\n";
        return -1;
    }
    
    return (String(argv[1]) == String(argv[2])) ? -1 : 0;
}
