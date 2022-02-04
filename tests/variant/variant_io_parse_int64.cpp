////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <util/text/String.hpp>
#include <util/type/Variant.hpp>
#include <stdlib.h>
#include <iostream>

int main(int argc, char* argv[])
{
    using namespace yq;
    
    if(argc < 2){
        std::cerr << "Insufficient Arguments!\n";
        return -1;
    }

    Meta::freeze();
    int64_t i   = atoll(argv[1]);
    String      s(argv[1]);
    Variant v   = Variant::parse_me(meta<int64_t>(), s);
    int64_t ii  = v.value<int64_t>();
    return (i == ii) ? 0 : -1;
}
