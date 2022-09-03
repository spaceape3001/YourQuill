////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ValueFile.hpp"
#include <basic/Logging.hpp>
#include <basic/XmlUtils.hpp>
#include <kernel/io/Strings.hpp>

namespace yq {
    bool    Value::ListFile::read(const XmlDocument&doc, std::string_view fname)
    {
        const XmlNode*  xn  = doc.first_node(szYQValues);
        if(!xn){
            yError() << "No appropriate root element!  In: '" << fname << "'";
            return false;
        }
        
        values  = read_children(xn, szValue, x_value);
        return true;
    }
    
    bool    Value::ListFile::write(XmlDocument&doc) const
    {
        XmlNode*  xroot  = doc.allocate_node(rapidxml::node_element, szYQUser);
        doc.append_node(xroot);
        
        write_children(xroot, szValue, values);
        return true;
    }
}
