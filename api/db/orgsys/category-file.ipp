////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace yq {

    void    Category::File::reset() 
    {
        *(Data*) this = Data{};
    }

    bool    Category::File::read(const XmlDocument&doc, std::string_view fname) 
    {
        const XmlNode*  xn  = doc.first_node(szYQCategory);
        if(!xn){
            yError() << "No appropriate root element!  In: '" << fname << "'";
            return false;
        }
        
        xn      = xn -> first_node(szCategory);
        if(!xn){
            yError() << "No category! In: " << fname << "'";
            return false;
        }
        
        name        = read_child(xn, szName, x_string);
        brief       = read_child(xn, szBrief, x_string);
        notes       = read_child(xn, szNotes, x_string);
        return true;
    }


    bool    Category::File::write(XmlDocument& doc) const 
    {
        XmlNode*  xroot  = doc.allocate_node(rapidxml::node_element, szYQCategory);
        doc.append_node(xroot);
        XmlNode*    xcategory   = doc.allocate_node(rapidxml::node_element, szCategory);
        xroot -> append_node(xcategory);
        write_child(xcategory, szName, name);
        if(!brief.empty())
            write_child(xcategory, szBrief, brief);
        if(notes.empty())
            write_child(xcategory, szNotes, notes);
        return true;
    }

}
