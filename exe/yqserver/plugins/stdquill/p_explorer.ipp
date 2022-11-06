////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/TextUtils.hpp>
#include <basic/DirUtils.hpp>

namespace {
    
    #if 0
    bool    isLess_Key_DocOrFoldStr(const DocOrFoldStr& a, const DocOrFoldStr& b)
    {
        return is_less_igCase(a.second, b.second);
    }
    #endif
    
    struct ExplorerEntry {
        enum Type {
            None    = 0,
            IsFolder,
            IsDocument
        };
        Document                    doc;
        Folder                      folder;
        Type                        type    = None;
        Image                       icon;
        std::string                 skey, name, label, suffix;
        std::vector<const Root*>    roots;
        
        ExplorerEntry(){}
    };
    
    #if 0
    bool    isLess_ExplorerEntry_label(const ExplorerEntry& a, const ExplorerEntry& b)
    {
        return is_less_igCase(a.label, b.label);
    }
    #endif
    
    bool    isLess_ExplorerEntry_skey(const ExplorerEntry& a, const ExplorerEntry& b)
    {
        return is_less_igCase(a.skey, b.skey);
    }

    #if 0
    bool    isLess_ExplorerEntry_name(const ExplorerEntry& a, const ExplorerEntry& b)
    {
        return is_less_igCase(a.name, b.name);
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

    std::vector<ExplorerEntry>  query_explorer_entries(Folder x, unsigned int opts=0)
    {
        static thread_local SQ  sd1("SELECT id,sk,name,icon,suffix FROM Documents WHERE folder=? and hidden=0 ORDER BY sk");
        static thread_local SQ  sd2("SELECT id,sk,name,icon,suffix FROM Documents WHERE folder=? ORDER BY sk");
        static thread_local SQ  sf1("SELECT id,sk,name,icon FROM Folders WHERE parent=? and hidden=0 ORDER BY sk");
        static thread_local SQ  sf2("SELECT id,sk,name,icon FROM Folders WHERE parent=? ORDER BY sk");
        
        std::vector<ExplorerEntry>  ret;
        
        SQ& sf  = (opts&HIDDEN) ? sf2 : sf1;
        auto af = sf.af();
        sf.bind(1, x.id);
        while(sf.step() == SqlQuery::Row){
            ExplorerEntry   ee;
            ee.folder   = { sf.v_uint64(1) };
            ee.type     = ExplorerEntry::IsFolder;
            ee.skey     = sf.v_string(2);
            ee.name     = sf.v_string(3);
            if(ee.name.empty())
                ee.label    = ee.skey;
            else
                ee.label    = ee.name;
            ee.icon     = Image{ sf.v_uint64(4) };
            ee.roots    = cdb::roots( ee.folder );
            ret.push_back(ee);
        }

        SQ& sd  = (opts&HIDDEN) ? sd2 : sd1;
        auto bf = sd.af();
        sd.bind(1, x.id);
        
        while(sd.step() == SqlQuery::Row){
            ExplorerEntry   ee;
            ee.doc      = { sd.v_uint64(1) };
            ee.skey     = sd.v_string(2);
            ee.name     = sd.v_string(3);
            ee.type     = ExplorerEntry::IsDocument;
            if(ee.name.empty())
                ee.label    = ee.skey;
            else
                ee.label    = ee.name;
            ee.icon     = Image{ sd.v_uint64(4) };
            ee.suffix   = sd.v_string(5);
            ee.roots    = cdb::roots( ee.doc );
            ret.push_back(ee);
        }
        
        std::stable_sort( ret.begin(), ret.end(), isLess_ExplorerEntry_skey);
        
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
            
        std::vector<ExplorerEntry>  entries = query_explorer_entries(x, HIDDEN);
        static const auto& icons  = file_extension_icons();
    
        if(x == top_folder()){
            h.title("/");
        } else {
            h.title(cdb::key(x));
        }
        
        h << "<div id=\"explorer\">\n";
    
        auto tac = h.table();
        auto iz = h.context().session.icon_size;
        html::columns(h, entries,  [&](const ExplorerEntry& ee){
            if(ee.type == ExplorerEntry::None)
                return ;
            if(ee.icon != Image{}){
                h << Thumbnail{ee.icon.id, iz };
            } else if(ee.type == ExplorerEntry::IsFolder){
                h << "<img src=\"/img/folder.svg\" class=\"" << iz << "\">";
            } else {
                std::string k   = icons.get(ee.suffix);
                if(!k.empty()){
                    h << "<img src=\"" << k << "\" class=\"" << iz << "\">";
                } else {
                    h << "<img src=\"/img/document.svg\" class=\"" << iz << "\">";
                }
            }
        }, [&](const ExplorerEntry& ee){
            if(ee.type == ExplorerEntry::None)
                return ;
            bool    isLink  = false;
            switch(ee.type){
            case ExplorerEntry::IsFolder:
                h << "<a href=\"/explore?folder=" << ee.folder.id << opts << "\">";
                isLink  = true;
                break;
            case ExplorerEntry::IsDocument:
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
}
