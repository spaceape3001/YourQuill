////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "TagFile.hpp"
#include <kernel/KeyValue.hpp>
#include <kernel/Strings.hpp>

namespace yq {
    void    Tag::File::reset() 
    {
        Data::reset();
    }

    #if TAG_XML_RESAVE
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
    #endif

    bool    Tag::File::read(KVTree&&attrs, std::string_view fname) 
    {
        name    = attrs.value(kv::key({"%", "name"}));
        brief   = attrs.value("brief");
        notes   = attrs.value("notes");
        leaf    = attrs.value("leaf");
        return true;
    }
    
    bool    Tag::File::write(KVTree&attrs) const 
    {
        if(!name.empty())
            attrs.set("%", name);
        if(!brief.empty())
            attrs.set("brief", brief);
        if(!notes.empty())
            attrs.set("notes", notes);
        if(!leaf.empty())
            attrs.set("leaf", leaf);
        return true;
    }
}

