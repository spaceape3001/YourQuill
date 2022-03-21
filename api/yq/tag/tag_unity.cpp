////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "TagFile.hpp"
#include <yq/bit/KeyValue.hpp>
#include <yq/text/Utils.hpp>

namespace yq {

    TagData& TagData::merge(const TagData&b, bool fOverride)
    {
        set_if_empty(leaf, b.leaf, fOverride);
        set_if_empty(name, b.name, fOverride);
        set_if_empty(brief, b.brief, fOverride);
        set_if_empty(notes, b.notes, fOverride);
        return *this;
    }

    void    TagData::reset() 
    {
        name.clear();
        leaf.clear();
        brief.clear();
        notes.clear();
    }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


    void    TagFile::reset() 
    {
        TagData::reset();
    }

    bool    TagFile::read(ByteArray&&buffer, const std::string_view& fname) 
    {
        KVTree          attrs;
        if(!attrs.parse(buffer, nullptr, true, fname))
            return false;

        name        = attrs.value(kv::key("%", "%tag", "tag", "%name", "name" ));
        notes       = attrs.value(kv::key("%note", "note", "notes", "%notes" ));
        brief       = attrs.value(kv::key("%desc", "brief", "desc", "%brief" ));
        leaf        = attrs.value(kv::key("%leaf", "leaf"));
        return true;
    }

    bool    TagFile::write(yq::Stream&chars) const
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

}
