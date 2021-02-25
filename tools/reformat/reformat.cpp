////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "db/StdFile.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    for(int i=1;i<argc;++i){
        StdBodyFile     sf;
        sf.load(argv[i]);
        sf.save();
    }
    return 0;
}


