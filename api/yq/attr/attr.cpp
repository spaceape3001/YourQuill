////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <kernel/attr/AttributeCDB.hpp>
#include <kernel/attr/AttributeDiff.hpp>
#include "AttributeArg.hpp"
#include "AttributeHtml.hpp"


#include <basic/preamble.hpp>
#include <kernel/bit/KeyValue.hpp>
#include <kernel/db/SQ.hpp>
#include <basic/StreamOps.hpp>
#include <basic/TextUtils.hpp>
#include <yq/web/WebContext.hpp>
#include <yq/web/WebHtml.hpp>


namespace yq {
////////////////////////////////////////////////////////////////////////////////

    namespace arg {
        Attribute attribute(std::string_view  arg_string)
        {
            arg_string   = trimmed(arg_string);
            if(arg_string.empty())
                return Attribute{};
            uint64_t    i = to_uint64( arg_string).value;
            if(cdb::exists_attribute(i))
                return Attribute{i};
            return Attribute{};
        }
        
        Attribute attribute(const WebContext&ctx, bool *detected)
        {
            std::string    k    = ctx.find_query("id");
            if(!k.empty())
                return attribute(k);
            k       = ctx.find_query("attr");
            if(!k.empty())
                return attribute(k);
            return Attribute{};
        }
        
        
        Attribute attribute(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return attribute(arg_string);
        }
        
        Attribute attribute(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return attribute(arg_string);
        }
    }

////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////

    namespace html {
        void    dev_table(WebHtml&h, const std::vector<Attribute>&table)
        {
            auto ta = h.table();
            ta << "<TR><TH>ID</TH><TH>Key</TH><TH>Value</TH></TR>\n";
            for(Attribute a : table){
                auto i = cdb::info(a);
                ta << "<tr><td>" << a.id << "</td><td>" << i.key << "</td><td>";
                html_escape_write(h, i.value);
                ta << "</td></tr>\n";
            }
        }
    }

////////////////////////////////////////////////////////////////////////////////
}


