////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "stdquill.hpp"
#include <yq/atom/Atom.hpp>
#include <yq/atom/Class.hpp>
#include <yq/atom/Field.hpp>

WebHtml&    operator<<(WebHtml&h, const DevID<Atom>&v)
{
    if(v.data)
        h << "<a href=\"/dev/atom?id=" << v.data.id << "\">";
    h << v.data.id;
    if(v.data)
        h << "</a>";
    return h;
}

WebHtml&    operator<<(WebHtml&h, const DevID<Class>&v)
{
    if(v.data)
        h << "<a href=\"/dev/class?id=" << v.data.id << "\">";
    h << v.data.id;
    if(v.data)
        h << "</a>";
    return h;
}

WebHtml&    operator<<(WebHtml&h, const DevID<Field>&v)
{
    if(v.data)
        h << "<a href=\"/dev/field?id=" << v.data.id << "\">";
    h << v.data.id;
    if(v.data)
        h << "</a>";
    return h;
}

