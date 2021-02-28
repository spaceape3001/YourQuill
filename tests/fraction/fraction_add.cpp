////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <stdlib.h>
#include <math/Fraction.hpp>

int main(int argc, char* argv[])
{
    if(argc < 7){
        std::cerr << "Insufficient arguments!\n";
        return -1;
    }

    Fraction    a(atoi(argv[1]), atoi(argv[2]));
    Fraction    b(atoi(argv[3]), atoi(argv[4]));
    Fraction    c(atoi(argv[5]), atoi(argv[6]));
    
    Fraction    d   = (a + b).simplified();
    
    return (c == d) ? 0 : -1;
}
