////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "TagFile.hpp"
#include <mithril/bit/KeyValue.hpp>
#include <basic/ErrorDB.hpp>

namespace yq::errors {
    using no_tag_in_file    = error_db::entry<"No tag found in file">;
}
    
namespace yq::mithril {

    void    Tag::File::reset() 
    {
        Data::reset();
    }

    #if TAG_XML_RESAVE
    std::error_code     Tag::File::read(const XmlDocument&doc, std::string_view fname) 
    {
        const XmlNode*  xn  = doc.first_node(szYQTag);
        if(!xn){
            yError() << "No appropriate root element!  In: '" << fname << "'";
            return errors::xml_no_root_element();
        }
        
        xn      = xn -> first_node(szTag);
        if(!xn){
            yError() << "No tag! In: " << fname << "'";
            return errors::no_tag_in_file();
        }
        
        name        = read_child(xn, "name", x_string);
        brief       = read_child(xn, "brief", x_string);
        notes       = read_child(xn, "notes", x_string);
        leaf        = read_child(xn, "leaf", x_string);
        return std::error_code(); 
    }
    #endif

    std::error_code     Tag::File::read(KVTree&&attrs, std::string_view) 
    {
        name    = attrs.value(kv::key({"%", "name"}));
        brief   = attrs.value("brief");
        notes   = attrs.value("notes");
        leaf    = attrs.value("leaf");
        return std::error_code(); 
    }
    
    std::error_code     Tag::File::write(KVTree&attrs) const 
    {
        if(!name.empty())
            attrs.set("name", name);
        if(!brief.empty())
            attrs.set("brief", brief);
        if(!notes.empty())
            attrs.set("notes", notes);
        if(!leaf.empty())
            attrs.set("leaf", leaf);
        return std::error_code(); 
    }
}

