////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "ValueHtml.hpp"
#include <mithril/bit/NKI.hpp>
#include <mithril/field/FieldCDB.hpp>
#include <mithril/field/FieldInfo.hpp>
#include <mithril/value/ValueCDB.hpp>
#include <mithril/web/WebContext.hpp>
#include <mithril/web/WebHtml.hpp>

namespace yq::mithril::html {
    void    dev_title(WebHtml& h, Value x, std::string_view extra)
    {
        auto t   = h.title();
        auto i  = cdb::info(x);
        auto fi = cdb::info(i.field);
        h << "Value \"" << fi.name << "\" (" << x.id << " &mdash; " << fi.key << ")";
        if(!extra.empty())
            h << ": " << extra;
    }
}
