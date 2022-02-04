////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <util/type/Variant.hpp>
#include <iostream>

bool    success()
{
    yq::Variant     v;
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
