////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <stdio.h>
#include <algorithm>

#include "meta/MetaValue.hpp"
#include "meta/Variant.hpp"
#include "util/Vector.hpp"
//#include "math/MathFwd.hpp"


int main(int argc, char* argv[])
{
    meta_freeze();
    Vector<const MetaValue*> all = MetaValue::all();
    size_t      mx  = 0;
    size_t      lx  = 0;
    for(const MetaValue* mt : all){
        mx  = std::max(mx, mt->name().size());
        lx  = std::max(lx, mt->label().size());
    }
    for(const MetaValue* mt : all){
        Variant var(mt);
        String  fmt = var.print();
        printf("%3u %-*s %-*s %3u bytes def=%s\n", mt->id(), (int) mx, (const char*) mt->name().c_str(), 
            (int) lx, (const char*) mt->label().c_str(), (unsigned) mt->size(), (const char*) fmt.c_str());
    }
    return 0;
}
