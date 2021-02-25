////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "ClassFile.hpp"



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

ClassFile::FieldWriter         ClassFile::field(const String&k, bool fCreate)
{
    return FieldWriter(first(zAttr, k, fCreate));
}

ClassFile::FieldReader         ClassFile::field(const String&k) const
{
    return FieldReader(first(zAttr, k));
}



Vector<ClassFile::FieldInfo>    ClassFile::fields() const
{
    Vector<FieldInfo>    ret;
    for(const Attribute* a : all(zAttr))
        if(a->data.empty()){
            continue;
        ret << FieldInfo(a);
    }
    return ret;
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

StringSet           ClassFile::reverse() const
{
    return values_set(zReverse);
}

void                ClassFile::reverse(const StringSet&v)
{
    set_set(zReverse, v);
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


ClassFile::FieldInfo::FieldInfo(const Attribute*a)
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
    
    for(const Attribute* v : a->all(zValue)){
        if(v->data.empty())
            continue;
        values << ValueInfo(v);
    }
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

String          ClassFile::FieldReader::key() const
{
    return m_ptr ? m_ptr -> data.strip_spaces() : String();
}

String          ClassFile::FieldReader::name() const
{
    return m_ptr ? m_ptr -> value(zName) : String();
}

String          ClassFile::FieldReader::desc() const
{
    return m_ptr ? m_ptr -> value(zBrief) : String();
}

String          ClassFile::FieldReader::plural() const
{
    return m_ptr ? m_ptr -> value(zPlural) : String();
}

StringSet       ClassFile::FieldReader::types() const
{
    return m_ptr ? m_ptr -> values_set(zType) : StringSet();
}

StringSet       ClassFile::FieldReader::atoms() const
{
    return m_ptr ? m_ptr -> values_set(zAtom) : StringSet();
}


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    
void        ClassFile::FieldWriter::name(const String&v)
{
    if(m_ptr)
        m_ptr -> set(zName, v, true);
}

void        ClassFile::FieldWriter::desc(const String&v)
{
    if(m_ptr)
        m_ptr -> set(zBrief, v, true);
}

void        ClassFile::FieldWriter::plural(const String&v)
{
    if(m_ptr)
        m_ptr -> set(zPlural, v, true);
}

void        ClassFile::FieldWriter::types(const StringSet&v)
{
    if(m_ptr)
        m_ptr -> set_set(zType, v);
}

void        ClassFile::FieldWriter::atoms(const StringSet&v)
{
    if(m_ptr)
        m_ptr -> set_set(zAtom, v);
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

