////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "util/Strings.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    if(argc<3){
        std::cerr << "Insufficent Arguments!\n";
        return -1;
    }
    
    return (String(argv[1]) == String(argv[2])) ? 0 : -1;
}
