////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "ClassFile.hpp"


Vector<ClassFile::AttrInfo>    ClassFile::attrs() const
{
    Vector<AttrInfo>    ret;
    for(const Attribute* a : all(zAttr))
        if(a->data.empty()){
            continue;
        ret << AttrInfo(a);
    }
    return ret;
}


String      ClassFile::brief() const
{
    return value(zBrief);
}

void        ClassFile::brief(const String&s)
{
    set(zBrief, s, true);
}

bool        ClassFile::edge() const
{
    return has(zSource) || has(zTarget);
}

String      ClassFile::name() const
{
    return value(zName);
}

void        ClassFile::name(const String&s)
{
    set(zName, s, true);
}


String      ClassFile::notes() const
{
    return value(zNote);
}

void        ClassFile::notes(const String&s)
{
    set(zNote, s, true);
}

String      ClassFile::plural() const
{
    return value(zPlural);
}

void        ClassFile::plural(const String&s)
{
    set(zPlural, s, true);
}

StringSet           ClassFile::source() const
{
    return values_set(zSource);
}

void                ClassFile::source(const StringSet&v)
{
    set_set(zSource, v);
}

StringSet           ClassFile::target() const
{
    return values_set(zTarget);
}

void                ClassFile::target(const StringSet&v)
{
    set_set(zTarget, v);
}

StringSet           ClassFile::use() const
{
    return values_set(zUse);
}

void                ClassFile::use(const StringSet&v)
{
    set_set(zUse, v);
}


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


ClassFile::AttrInfo::AttrInfo(const Attribute*a)
{
    assert(a);
    if(!a)
        return ;

    key     = a->data.strip_spaces();
    desc    = a->value(zBrief);
    plural  = a->value(zPlural);
    name    = a->value(zAttrName);
    types   = a->values_set(zType);
    atoms   = a->values_set(zAtom);
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

ClassFile::ValueInfo::ValueInfo(const Attribute*a)
{
    assert(a);
    if(!a)
        return ;
    
    value       = a->data;
    desc        = a->value(zBrief);
    type        = a->value(zType);
}

