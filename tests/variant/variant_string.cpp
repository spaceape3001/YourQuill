////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "yq/meta/Variant.hpp"
#include "yq/util/String.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    if(argc < 2){
        std::cerr << "Insufficient arguments!\n";
        return -1;
    }
    
    meta_freeze();
    String      str((const char8_t*) argv[1]);
    Variant     var((const char8_t*) argv[1]);
    
    if(str != var.value<String>().value)
        return -1;
    return 0;
}
