////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <http/XEntry.hpp>

namespace {
    
    #if 0
    bool    isLess_Key_DocOrFoldStr(const DocOrFoldStr& a, const DocOrFoldStr& b)
    {
        return is_less_igCase(a.second, b.second);
    }
    #endif

    bool    decode_do_roots(const WebContext&ctx)
    {
        std::string     s   = ctx.find_query("roots");
        if(s.empty())
            return false;
        return to_boolean(s).value;
    }
    
    void    p_explorer(WebHtml&h)
    {
        Folder  x       = folder(h);
        if(!x)
            x           = top_folder();
        bool  do_roots  = decode_do_roots(h.context());

        XEntry::Details     detail;
        detail.option.hidden       = true;
        if(do_roots)
            detail.option.roots    = true;

        detail.icon_size            = h.context().session.icon_size;
        {
            stream::Text    link(detail.link.folder);
            link << "/file/explore?";
            if(do_roots)
                link << "roots=1&";
            link << "folder=";
        }
        
        detail.option.merge     = true;
        detail.query.folders    = true;
        detail.query.documents  = true;
            
        std::vector<XEntry>  entries = XEntry::query(x, detail);
    
        if(x == top_folder()){
            h.title() << "/ : File Explorer";
        } else {
            h.title() << cdb::key(x) << " : File Explorer";
        }
        
        h << "<div id=\"explorer\">\n";
        {
            auto tac = h.table();
            html::columns(h, entries,  
                [&](const XEntry& ee)
                {
                    if(ee.type == XEntry::None)
                        return;
                    auto al = ee.write_link(h, detail);
                    ee.write_thumbnail(h, detail);
                }, 
                [&](const XEntry& ee){
                    if(ee.type == XEntry::None)
                        return ;
                    {
                        auto    isLink  = ee.write_link(h, detail);
                        html_escape_write(h, ee.skey);
                    }
                    if(do_roots){
                        h << "<br><font size=-2>[&nbsp;";
                        for(const Root* rt : ee.roots){
                            if(!rt)
                                continue;
                            h << rt->key << "&nbsp;";
                        }
                        h << "]</font>";
                    }
                }
            );
        }
        h << "</div>\n";
    }

    void    reg_explorer()
    {
        reg_webpage<p_explorer>("/file/explore");
    }
    YQ_INVOKE(reg_explorer();)
}
