////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

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

bool    TagFile::read(ByteArray&&buffer, const std::string& fname) 
{
    KVTree          attrs;
    if(!attrs.parse(buffer, nullptr, true, fname))
        return false;

    name        = attrs.value({"%", "%tag", "tag", "%name", "name" });
    notes       = attrs.value({"%note", "note", "notes", "%notes" });
    brief       = attrs.value({"%desc", "brief", "desc", "%brief" });
    leaf        = attrs.value({"%leaf", "leaf"});
    return true;
}

bool    TagFile::write(Stream&chars) const
{
    KVTree        attrs;
    if(!name.empty())
        attrs << KeyValue("%", name);
    if(!brief.empty())
        attrs << KeyValue("desc", brief);
    if(!leaf.empty())
        attrs << KeyValue("leaf", leaf);
    if(!notes.empty())
        attrs << KeyValue("note", notes);
    attrs.write(chars);
    return true;
}

