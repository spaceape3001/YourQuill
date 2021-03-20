////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "LeafFile.hpp"
#include <util/Set.hpp>


String      LeafFile::abbr() const
{
    return value(zAbbr);
}

void        LeafFile::abbr(const String&s)
{
    set(zAbbr, s, true);
}

String      LeafFile::brief() const
{
    return value(zBrief);
}

void        LeafFile::brief(const String&s)
{
    set(zBrief, s, true);
}

StringSet   LeafFile::classes() const
{
    return values_set(zClass);
}

void        LeafFile::classes(const StringSet&s)
{
    set_set(zClass, s);
}

Context     LeafFile::context() const
{
    return Context(format(), body);
}


Format      LeafFile::format() const
{
    const KeyValue* a = first(zFormat);
    if(!a)
        return defFormat;
    auto r = Format::value_for(a->data);
    if(r.good)
        return r.value;
    return defFormat;
}

void        LeafFile::format(Format f)
{
    set(zFormat, f.key(), true);
}

String      LeafFile::notes() const
{
    return value(zNote);
}

void        LeafFile::notes(const String&s)
{
    set(zNote, s, true);
}

StringSet   LeafFile::tags() const
{
    return values_set(zTag);
}

void        LeafFile::tags(const StringSet&s)
{
    set_set(zTag, s);
}

String      LeafFile::title() const
{
    return value(zTitle);
}

void        LeafFile::title(const String&s)
{
    set(zTitle, s, true);
}
