////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <stdio.h>
#include <algorithm>

#include <util/text/String.hpp>
#include <util/type/Variant.hpp>
#include <util/collection/Vector.hpp>
//#include "math/MathFwd.hpp"

using namespace yq;


int main(int argc, char* argv[])
{
    Meta::freeze();
    
    Vector<const TypeInfo*> all = TypeInfo::all();
    size_t      mx  = 0;
    size_t      lx  = 0;
    for(const TypeInfo* mt : all){
        mx  = std::max(mx, mt->name().size());
        lx  = std::max(lx, mt->label().size());
    }
    for(const TypeInfo* mt : all){
        Variant var(mt);
        String  fmt     = var.printable();
        String  name    = mt->name();
        String  label   = mt->label();
        printf("%3u %-*s %-*s %3u bytes def=%s\n", mt->id(), (int) mx, (const char*) name.c_str(), 
            (int) lx, (const char*) label.c_str(), (unsigned) mt->size(), (const char*) fmt.c_str());
    }
    return 0;
}
