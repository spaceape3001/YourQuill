////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "tag_file.hpp"

namespace yq {

    void    Tag::File::reset() 
    {
        Data::reset();
    }

    bool    Tag::File::read(const XmlDocument&doc, std::string_view fname) 
    {
        const XmlNode*  xn  = doc.first_node(szYQTag);
        if(!xn){
            yError() << "No appropriate root element!  In: '" << fname << "'";
            return false;
        }
        
        xn      = xn -> first_node(szTag);
        if(!xn){
            yError() << "No tag! In: " << fname << "'";
            return false;
        }
        
        name        = read_child(xn, szName, x_string);
        brief       = read_child(xn, szBrief, x_string);
        notes       = read_child(xn, szNotes, x_string);
        leaf        = read_child(xn, szLeaf, x_string);
        return true;
    }

    bool    Tag::File::read(ByteArray&&buffer, const std::string_view& fname) 
    {
        if(starts(buffer.as_view(), "<?xml"))
            return XmlFile::read(std::move(buffer), fname);
    
    
        KVTree          attrs;
        if(!attrs.parse(buffer, nullptr, true, fname))
            return false;

        name        = attrs.value(kv::key("%", "%tag", "tag", "%name", "name" ));
        notes       = attrs.value(kv::key("%note", "note", "notes", "%notes" ));
        brief       = attrs.value(kv::key("%desc", "brief", "desc", "%brief" ));
        leaf        = attrs.value(kv::key("%leaf", "leaf"));
        return true;
    }

    bool    Tag::File::write(XmlDocument& doc) const 
    {
        xml_start(doc);
        
        XmlNode*  xroot  = doc.allocate_node(rapidxml::node_element, szYQTag);
        doc.append_node(xroot);
        XmlNode*    xtag   = doc.allocate_node(rapidxml::node_element, szTag);
        xroot -> append_node(xtag);
        write_child(xtag, szName, name);
        if(!brief.empty())
            write_child(xtag, szBrief, brief);
        if(notes.empty())
            write_child(xtag, szNotes, notes);
        if(!leaf.empty())
            write_child(xtag, szLeaf, leaf);
        return true;
    }

#if 0
    bool    Tag::File::write(yq::Stream&chars) const
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
#endif

}
