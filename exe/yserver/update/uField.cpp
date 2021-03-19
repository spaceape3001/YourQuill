////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "uField.hpp"
#include <util/Set.hpp>

UField::UField(Field f) : key(cdb::key(f)), id(f.id), implied(false)
{
}

UField&             uget(Field f)
{
    static Vector<UField*>      data(20480, nullptr);
    data.resize_if_under(f.id+1,256,nullptr);
    UField*& p = data[f.id];
    if(!p)
        p       = new UField(f);
    return *p;
}
