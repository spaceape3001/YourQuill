////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "TagFile.hpp"
#include <db/bit/Attribute.hpp>

TagData& TagData::merge(const TagData&b, bool fOverride)
{
    leaf.set_if(b.leaf, fOverride);
    name.set_if(b.name, fOverride);
    brief.set_if(b.brief, fOverride);
    notes.set_if(b.notes, fOverride);
    return *this;
}

void    TagData::reset() 
{
    name.clear();
    leaf.clear();
    brief.clear();
    notes.clear();
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


void    TagFile::reset() 
{
    TagData::reset();
}

bool    TagFile::read(Vector<char>&buffer, const std::string& fname) 
{
    AttrTree        attrs;
    if(!attrs.parse(buffer, nullptr, true, fname))
        return false;

    name        = attrs.value({"%", "%tag", "tag", "%name", "name" });
    notes       = attrs.value({"%note", "note", "notes", "%notes" });
    brief       = attrs.value({"%desc", "brief", "desc", "%brief" });
    leaf        = attrs.value({"%leaf", "leaf"});
    return true;
}

bool    TagFile::write(Vector<char>&chars) 
{
    AttrTree        attrs;
    if(!name.empty())
        attrs << Attribute("%", name);
    if(!brief.empty())
        attrs << Attribute("desc", brief);
    if(!leaf.empty())
        attrs << Attribute("leaf", leaf);
    if(!notes.empty())
        attrs << Attribute("note", notes);
    attrs.write(chars);
    return true;
}

