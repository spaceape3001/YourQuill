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
    
    string_map_t    make_file_extension_icons()
    {
        string_map_t    ret;
        
        dir::for_all_children(wksp::shared_all("www/img/ext"sv), dir::NO_DIRS, [&](const std::filesystem::path& p){
            auto sfx = p.extension();
            if(!is_similar(sfx.c_str(), ".svg"))
                return;
            auto file           = p.filename();
            auto key            = p.stem();
            ret[key.c_str()]    = "/img/ext/"s + file.c_str();
        });
        
        return ret;
    }
    
    const string_map_t& file_extension_icons()
    {
        static string_map_t     s_data  = make_file_extension_icons();
        return s_data;
    }

    std::vector<XEntry>  query_explorer_entries(Folder x, unsigned int opts=0)
    {
        std::vector<XEntry> ret;
        XEntry::query_folders(ret, x, opts);
        XEntry::query_documents(ret, x, opts);
        std::stable_sort( ret.begin(), ret.end(), XEntry::isLess_skey);
        return ret;
    }
    
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
        std::string opts    = do_roots ? "&roots=1" : "";
            
        uint64_t    opt    = HIDDEN;
        if(do_roots)
            opt |= ROOTS;
            
        std::vector<XEntry>  entries = query_explorer_entries(x, opt);
        static const auto& icons  = file_extension_icons();
    
        if(x == top_folder()){
            h.title("/");
        } else {
            h.title(cdb::key(x));
        }
        
        h << "<div id=\"explorer\">\n";
    
        auto tac = h.table();
        auto iz = h.context().session.icon_size;
        html::columns(h, entries,  [&](const XEntry& ee){
            if(ee.type == XEntry::None)
                return ;
            if(ee.icon != Image{}){
                h << Thumbnail{ee.icon.id, iz };
            } else if(ee.type == XEntry::IsFolder){
                h << "<img src=\"/img/folder.svg\" class=\"" << iz << "\">";
            } else {
                std::string k   = icons.get(ee.suffix);
                if(!k.empty()){
                    h << "<img src=\"" << k << "\" class=\"" << iz << "\">";
                } else {
                    h << "<img src=\"/img/document.svg\" class=\"" << iz << "\">";
                }
            }
        }, [&](const XEntry& ee){
            if(ee.type == XEntry::None)
                return ;
            bool    isLink  = false;
            switch(ee.type){
            case XEntry::IsFolder:
                h << "<a href=\"/explore?folder=" << ee.folder.id << opts << "\">";
                isLink  = true;
                break;
            case XEntry::IsDocument:
                break;
            default:
                break;
            }
            
            html_escape_write(h, ee.skey);
            if(isLink)
                h << "</a>";
                
            if(do_roots){
                h << "<br><font size=-2>[&nbsp;";
                for(const Root* rt : ee.roots){
                    if(!rt)
                        continue;
                    h << rt->key << "&nbsp;";
                }
                h << "]</font>";
            }
        });
        
        h << "</div>\n";
    }

    void    reg_explorer()
    {
        reg_webpage<p_explorer>("/explore");
    }
    YQ_INVOKE(reg_explorer();)
}
