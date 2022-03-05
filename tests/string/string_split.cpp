////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "util/text/Utils.hpp"
#include "util/collection/Vector.hpp"
#include <iostream>

using namespace yq;

int main(int,char*[])
{
    auto  output  = split("1 2 3", ' ');
    if(output.size() != 3){
        std::cerr << "Size is not 3!\n";
        return -1;
    }
    if(output[0] != "1"){
        std::cerr << "First item failed to compare!\n";
        std::cerr << "'" << output[0] << "'";
        return -1;
    }
    if(output[1] != "2"){
        std::cerr << "Second item failed to compare!\n";
        return -1;
    }
    if(output[2] != "3"){
        std::cerr << "Third item failed to compare!\n";
        return -1;
    }
    return 0;
}
