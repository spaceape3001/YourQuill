////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "file.hpp"

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


    bool    Tag::File::write(XmlDocument& doc) const 
    {
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

}
