////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "stdquill.hpp"

#if 0
WebHtml&    operator<<(WebHtml&h, const Dev<Leaf>&v)
{
    std::string n       = cdb::name(v.data);
    if(n.empty())
        n   = "(no-name)";
    if(v.data)
        h << "<a href=\"/dev/leaf?id=" << v.data.id << "\">";
    h << "[" << v.data.id << "] " << n;
    if(v.data)
        h << "</a>";
    return h;
}
#endif

WebHtml&    operator<<(WebHtml&h, const DevID<Leaf>&v)
{
    if(v.data)
        h << "<a href=\"/dev/leaf?id=" << v.data.id << "\">";
    h << v.data.id;
    if(v.data)
        h << "</a>";
    return h;
}
