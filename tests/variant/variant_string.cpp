////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <util/text/String.hpp>
#include <util/type/Variant.hpp>
#include <iostream>

int main(int argc, char* argv[])
{
    using namespace yq;
    
    if(argc < 2){
        std::cerr << "Insufficient arguments!\n";
        return -1;
    }
    
    Meta::freeze();
    String      str((const char8_t*) argv[1]);
    Variant     var((const char8_t*) argv[1]);
    
    std::cout << "Variant is " << var.printable() << "\n";
    
    if(str != var.value<String>().value)
        return -1;
    return 0;
}
