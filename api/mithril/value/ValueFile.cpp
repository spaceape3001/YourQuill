////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "ValueFile.hpp"
#include <yq/core/Logging.hpp>
#include <yq/xml/XmlUtils.hpp>
#include <mithril/io/Strings.hpp>
#include <mithril/errors.hpp>
#include <yq/errors.hpp>

namespace yq::mithril {
    std::error_code Value::ListFile::read(const XmlDocument&doc, std::string_view fname)
    {
        const XmlNode*  xn  = doc.first_node(szYQValues);
        if(!xn){
            yError() << "No appropriate root_dir element!  In: '" << fname << "'";
            return errors::xml_no_root_element();
        }
        
        values  = read_children(*xn, szValue, x_value);
        return std::error_code();
    }
    
    std::error_code Value::ListFile::write(XmlDocument&doc) const
    {
        XmlNode*  xroot  = doc.allocate_element(szYQUser);
        doc.append_node(xroot);
        
        write_children(*xroot, szValue, values);
        return std::error_code();
    }
}
