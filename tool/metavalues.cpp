////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
//  Utility for printing out the available metavalue types
//
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <stdio.h>
#include <algorithm>

#include <yq/basic/Any.hpp>
#include <yq/text/copy.hpp>

#include <yq/container/Vector.hpp>

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
        Any var(mt);
        std::string  fmt     = var.printable();
        std::string  name    = copy(mt->name());
        std::string  label   = copy(mt->label());
        printf("%4lu %-*s %-*s %3u bytes def=%s\n", mt->id(), (int) mx, (const char*) name.c_str(), 
            (int) lx, (const char*) label.c_str(), (unsigned) mt->size(), (const char*) fmt.c_str());
    }
    
    
    
    return 0;
}
