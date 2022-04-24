////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "file.hpp"

namespace yq {

    void Field::File::reset() 
    {
        (*(Data*) this) = Data{};
    }

    bool    Field::File::read(const XmlDocument& doc, std::string_view fname) 
    {
        const XmlNode*  xn  = doc.first_node(szYQField);
        if(!xn){
            yError() << "No appropriate root element!  In: '" << fname << "'";
            return false;
        }
        
        xn      = xn -> first_node(szField);
        if(!xn){
            yError() << "No field! In: " << fname << "'";
            return false;
        }

        pkey            = read_child(xn, szPKey, x_sstring);
        name            = read_child(xn, szName, x_sstring);
        plural          = read_child(xn, szPlural, x_sstring);
        brief           = read_child(xn, szBrief, x_sstring);
        notes           = read_child(xn, szNotes, x_sstring);
        category        = read_child(xn, szCategory, x_sstring);
        aliases         = read_child_string_set(xn, szAlias);
        classes         = read_child_string_set(xn, szClass);
        types           = read_child_string_set(xn, szType);
        atoms           = read_child_string_set(xn, szAtom);
        expected        = read_child(xn, szExpected, x_sstring);
        tags            = read_child_string_set(xn, szTag);
        multiplicity    = read_child(xn, szMultiple, x_enum<Multiplicity>);
        restriction     = read_child(xn, szRestrict, x_enum<Restriction>);
        max_count       = read_child(xn, szMaxCount, x_uint64).value;
        return true;
    }
    
    bool    Field::File::write(XmlDocument& doc) const 
    {
        XmlNode*  xroot  = doc.allocate_node(rapidxml::node_element, szYQField);
        doc.append_node(xroot);
        XmlNode*    xn   = doc.allocate_node(rapidxml::node_element, szField);
        xroot -> append_node(xn);
        
        write_child(xn, szName, name);
        write_children(xn, szTag, tags);
        if(!plural.empty())
            write_child(xn, szPlural, plural);
        if(!pkey.empty())
            write_child(xn, szPKey, pkey);
        if(!brief.empty())
            write_child(xn, szBrief, brief);
        if(!notes.empty())
            write_child(xn, szNotes, notes);
        if(!category.empty())
            write_child(xn, szCategory, category);
        write_children(xn, szAlias, aliases);
        write_children(xn, szClass, classes);
        write_children(xn, szType, types);
        write_children(xn, szAtom, atoms);
        if(!expected.empty())
            write_child(xn, szExpected, expected);
        if(multiplicity != Multiplicity())
            write_child(xn, szMultiple, multiplicity);
        if(restriction != Restriction())
            write_child(xn, szRestrict, restriction);
        if(max_count)
            write_child(xn, szMaxCount, max_count);
        return true;
    }
}
