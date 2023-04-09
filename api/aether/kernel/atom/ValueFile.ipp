////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ValueFile.hpp"
#include <basic/Logging.hpp>
#include <io/XmlUtils.hpp>
#include <aether/kernel/io/Strings.hpp>
#include <io/errors.hpp>

namespace yq {

    std::error_code Value::ListFile::read(const XmlDocument&doc, std::string_view fname)
    {
        const XmlNode*  xn  = doc.first_node(szYQValues);
        if(!xn){
            yError() << "No appropriate root element!  In: '" << fname << "'";
            return errors::xml_no_root_element();
        }
        
        values  = read_children(xn, szValue, x_value);
        return std::error_code();
    }
    
    std::error_code Value::ListFile::write(XmlDocument&doc) const
    {
        XmlNode*  xroot  = doc.allocate_node(rapidxml::node_element, szYQUser);
        doc.append_node(xroot);
        
        write_children(xroot, szValue, values);
        return std::error_code();
    }
}
