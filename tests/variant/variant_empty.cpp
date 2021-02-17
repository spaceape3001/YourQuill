////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "yq/meta/Variant.hpp"
#include <iostream>

bool    success()
{
    Variant     v;
    if(v.type().id() != 0)
        return false;
    if(v != v)
        return false;
    if(!(v == v))
        return false;
    return true;
}

int main()
{
    return success() ? 0 : -1;
}
