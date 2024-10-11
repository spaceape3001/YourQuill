////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RootHtml.hpp"
#include <yq/net/HttpParser.hpp>
#include <mithril/root/RootArg.hpp>
#include <mithril/web/WebContext.hpp>
#include <mithril/web/WebHtml.hpp>
#include <mithril/root/RootDir.hpp>
#include <mithril/root/RootCDB.hpp>
#include <mithril/wksp/Workspace.hpp>

namespace yq::mithril::html {
    WebHtml&    operator<<(WebHtml&h, Dev<const RootDir*> v)
    {
        if(!v.data){
            h << "(null root_dir)";
            return h;
        }
        
        h << "<a href=\"/dev/root_dir?id=" << v.data->id << "\">[" << v.data->id << "] " << v.data->key << "</a>";
        return h;
    }

    WebHtml&    operator<<(WebHtml& h, DevID<const RootDir*> v)
    {
        if(!v.data){
            h << "(null root_dir)";
            return h;
        }
        
        h << "<a href=\"/dev/root_dir?id=" << v.data->id << "\">" << v.data->id << "</a>";
        return h;
    }
    

    WebHtml&    operator<<(WebHtml&h, const ControlRoot&v)
    {
        auto& ctx   = h.context();
        
        auto qr = ctx.strip_query("root_dir");
        const RootDir* rt  = yq::mithril::arg::root_dir(qr.value);
        
        Url newurl      = copy(ctx.url);
        newurl.query    = qr.rest;
        
        h << "<table align=\"right\" class=\"root_dirs\">\n";
        h << "<tr><td class=\"root_dirs\">";
        if(rt)
            h << "<a href=\"" << newurl << "\">";
        h << "(Fused)";
        if(rt)
            h << "</a>";
        h << "</td></tr><tr><td class=\"root_dirs\"><hr width=\"10%\"></td></tr>\n";
        
        for(const RootDir* r : wksp::root_dirs()){
            newurl.query    = qr.rest;
            if(newurl.query.empty())
                newurl.query   = "?root_dir=";
            else
                newurl.query  += "&root_dir=";
            newurl.query += r->key;
            h << "<tr><td class=\"root_dirs\">";
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
        const RootDir* re = v.root_dir;
        if(!re)
            re  = h.context().def_root(v.role);
        if(!v.msg.empty()){
            html_escape_write(h, v.msg);
            h << "<br>";
        }
            
        for(const RootDir* rt : wksp::root_dirs()){
            h << "<input type=\"radio\" name=\"root_dir\" id=\"" << rt->key << "\" value=\"" << rt->id << "\"";
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

    void    dev_table(WebHtml&h, const std::vector<const RootDir*>& root_dirs)
    {
        auto _tab = h.table();
        h << "<tr><th>ID</th><th>Key</th><th>Name</th><th>Path</th></tr>\n";
        for(const RootDir* r : root_dirs){
            if(!r)
                continue;
            h << "<tr><td>" << dev_id(r) <<  "</td><td>" << r->key << "</td><td>" << r->name << "</td><td>" << r->path << "</td></tr>\n";
        }
    }

    void    dev_title(WebHtml& h, const RootDir* rt, std::string_view extra)
    {
        auto t = h.title();
        h << "Root \"" << rt->name << " (" << rt->id << " &mdash; " << rt->path  << ")";
        if(!extra.empty())
            h << ": " << extra;
    }
}
