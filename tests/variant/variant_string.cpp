////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <util/text/Utils.hpp>
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
    std::string str(argv[1]);
    Variant     var(argv[1]);
    
    std::cout << "Variant is " << var.printable() << "\n";
    
    if(str != var.value<std::string>().value)
        return -1;
    return 0;
}
