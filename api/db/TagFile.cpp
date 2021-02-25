////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "TagFile.hpp"


String          TagFile::brief() const
{
    return value(zBrief);
}

void            TagFile::brief(const String& s)
{
    set(zBrief, s, true);
}

String          TagFile::leaf() const
{
    return value(zLeaf);
}

void            TagFile::leaf(const String&s)
{
    set(zLeaf, s, true);
}


String          TagFile::name() const
{
    return value(zName);
}

void            TagFile::name(const String&s)
{
    set(zName, s, true);
}

String          TagFile::notes() const
{
    return value(zNote);
}

void            TagFile::notes(const String& s)
{
    set(zNote, s, true);
}
