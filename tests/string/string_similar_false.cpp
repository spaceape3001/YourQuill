////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "util/String.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    if(argc<3){
        std::cerr << "Insufficent Arguments!\n";
        return -1;
    }
    
    return is_similar(String(argv[1]), String(argv[2])) ? -1 : 0;
}
