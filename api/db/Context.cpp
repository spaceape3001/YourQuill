////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Context.hpp"
#include "FileStrings.hpp"
#include <util/XmlUtils.hpp>


Context     x_context(const XmlNode* xn)
{
    auto    fmt = read_attribute(xn, szFormat, x_enum<Format>);
    Context ret;
    ret.format  = fmt.value;
    ret.data    = x_string(xn);
    return ret;
}

void                write_xn(XmlNode* xn, const Context&v)
{
    if(v.format != Format())
        write_attribute(xn, szFormat, v.format);
    if(!v.data.empty()){
        xn->append_node(
            xn->document()->allocate_node(
                rapidxml::node_data, nullptr, v.data.c_str(), 0ULL, v.data.size()
            )
        );
    }
}
