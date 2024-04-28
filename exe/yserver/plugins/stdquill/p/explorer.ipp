////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/html/XEntry.hpp>

namespace {

    //  -----------------------------------------------------------------------
    //      UTILITIES
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      REST API
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      PAGES
    //  -----------------------------------------------------------------------
    

        #if 0
        bool    isLess_Key_DocOrFoldStr(const DocOrFoldStr& a, const DocOrFoldStr& b)
        {
            return is_less_igCase(a.second, b.second);
        }
        #endif

        bool    decode_do_roots(const WebContext&ctx)
        {
            std::string     s   = ctx.find_query("root_dirs");
            if(s.empty())
                return false;
            return to_boolean(s).value_or(false);
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
                detail.option.root_dirs    = true;

            detail.icon_size            = h.context().session.icon_size;
            {
                stream::Text    link(detail.link.folder);
                link << "/file/explore?";
                if(do_roots)
                    link << "root_dirs=1&";
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
                            for(const RootDir* rt : ee.root_dirs){
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
    
    //  -----------------------------------------------------------------------
    //      ADMIN
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      DEV
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      REGISTRATION
    //  -----------------------------------------------------------------------

        void reg_explorer_pages()
        {
            reg_webpage<p_explorer>("/file/explore");
        }
}
