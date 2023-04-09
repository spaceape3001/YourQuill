////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RootHtml.hpp"
#include <basic/HttpParser.hpp>
#include <aether/http/file/RootArg.hpp>
#include <aether/http/web/WebContext.hpp>
#include <aether/http/web/WebHtml.hpp>
#include <aether/kernel/file/Root.hpp>
#include <aether/kernel/file/RootCDB.hpp>
#include <aether/kernel/wksp/Workspace.hpp>

namespace yq {
    namespace html {
        WebHtml&    operator<<(WebHtml&h, Dev<const Root*> v)
        {
            if(!v.data){
                h << "(null root)";
                return h;
            }
            
            h << "<a href=\"/dev/root?id=" << v.data->id << "\">[" << v.data->id << "] " << v.data->key << "</a>";
            return h;
        }

        WebHtml&    operator<<(WebHtml& h, DevID<const Root*> v)
        {
            if(!v.data){
                h << "(null root)";
                return h;
            }
            
            h << "<a href=\"/dev/root?id=" << v.data->id << "\">" << v.data->id << "</a>";
            return h;
        }
        

        WebHtml&    operator<<(WebHtml&h, const ControlRoot&v)
        {
            auto& ctx   = h.context();
            
            auto qr = ctx.strip_query("root");
            const Root* rt  = yq::arg::root(qr.value);
            
            Url newurl      = copy(ctx.url);
            newurl.query    = qr.rest;
            
            h << "<table align=\"right\" class=\"roots\">\n";
            h << "<tr><td class=\"roots\">";
            if(rt)
                h << "<a href=\"" << newurl << "\">";
            h << "(Fused)";
            if(rt)
                h << "</a>";
            h << "</td></tr><tr><td class=\"roots\"><hr width=\"10%\"></td></tr>\n";
            
            for(const Root* r : wksp::roots()){
                newurl.query    = qr.rest;
                if(newurl.query.empty())
                    newurl.query   = "?root=";
                else
                    newurl.query  += "&root=";
                newurl.query += r->key;
                h << "<tr><td class=\"roots\">";
                if(r != rt)
                    h << "<a href=\"" << newurl << "\">";
                h << r -> name;
                if(r != rt)
                    h << "</a>";
                h  << "</td></tr>\n";
            }
            h << "</table>\n";
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, const IRoot&v)
        {
            const Root* re = v.root;
            if(!re)
                re  = h.context().def_root(v.role);
            if(!v.msg.empty()){
                html_escape_write(h, v.msg);
                h << "<br>";
            }
                
            for(const Root* rt : wksp::roots()){
                h << "<input type=\"radio\" name=\"root\" id=\"" << rt->key << "\" value=\"" << rt->id << "\"";
                if(!rt->is_good(v.role, v.access))
                    h << " disabled=\"true\"";
                h   << "><label for=\"" << rt->key << "\">";
                if(rt == re)
                    h << "<b><u>";
                h << "[" << rt->key << "] " << rt->name;
                if(rt == re)
                    h << "</u></b>";
                h  << "</label><br>\n";
            }
            return h;
        }

        void    dev_table(WebHtml&h, const std::vector<const Root*>& roots)
        {
            auto _tab = h.table();
            h << "<tr><th>ID</th><th>Key</th><th>Name</th><th>Path</th></tr>\n";
            for(const Root* r : roots){
                if(!r)
                    continue;
                h << "<tr><td>" << dev_id(r) <<  "</td><td>" << r->key << "</td><td>" << r->name << "</td><td>" << r->path << "</td></tr>\n";
            }
        }

    }
}
