////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "XEntry.hpp"
#include <0/basic/CollectionUtils.hpp>
#include <0/basic/TextUtils.hpp>
#include <mithril/document/DocumentCDB.hpp>
#include <mithril/folder/FolderCDB.hpp>
#include <mithril/leaf/LeafCDB.hpp>
#include <mithril/wksp/CacheQuery.hpp>
#include <mithril/wksp/Workspace.hpp>
#include <mithril/document/DocumentHtml.hpp>

namespace yq::mithril {

    bool XEntry::isLess_label(const XEntry& a, const XEntry& b)
    {
        return is_less_igCase(a.label, b.label);
    }

    bool XEntry::isLess_skey(const XEntry& a, const XEntry& b)
    {
        return is_less_igCase(a.skey, b.skey);
    }

    bool XEntry::isLess_name(const XEntry& a, const XEntry& b)
    {
        return is_less_igCase(a.name, b.name);
    }

    bool XEntry::isLess_title(const XEntry& a, const XEntry& b)
    {
        return is_less_igCase(a.title, b.title);
    }

    ////////////////////////////////////////////////////////////////////////////////

    std::string_view    XEntry::Details::get_link(Type t) const
    {
        switch(t){
        case IsFolder:
            return link.folder;
        case IsDocument:
            return link.document;
        case IsLeaf:
            return link.leaf;
        default:
            return std::string_view();
        }
    }
    
    bool                XEntry::Details::get_query(Type t) const
    {
        switch(t){
        case IsFolder:
            return query.folders;
        case IsDocument:
            return query.documents;
        case IsLeaf:
            return query.leafs;
        default:
            return false;
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    
    XEntry::XEntry(const XEntry&) = default;
    XEntry::XEntry(XEntry&&) = default;
    XEntry::~XEntry() = default;
    XEntry& XEntry::operator=(const XEntry&) = default;
    XEntry& XEntry::operator=(XEntry&&) = default;

    //XEntry::XEntry(Leaf);
    //XEntry::XEntry(Document);
    //XEntry::XEntry(Folder);

    std::string_view    XEntry::link(const Details&dt) const
    {
        return dt.get_link(type);
    }

    WebAutoClose    XEntry::write_link(class WebHtml&h, const Details& dt) const
    {
        std::string_view    s   = dt.get_link(type);
        if(s.empty())
            return WebAutoClose();
        if(key.empty())
            return WebAutoClose();
        h << "<a href=\"" << s;
        html_escape_write(h, key);
        h << "\">";
        return WebAutoClose(h, "</a>");
    }

    void    XEntry::write_thumbnail(class WebHtml&h, const Details& dt) const
    {
        static const auto& icons  = html::file_extension_icons();
        
        if(icon != Image{}){
            h << Thumbnail{ icon.id, dt.icon_size };
            return;
        }
        
        switch(type){
        case IsFolder:
            if(icon != Image{}){
                h << Thumbnail{ icon.id, dt.icon_size };
            } else 
                h << "<img src=\"/img/folder.svg\" class=\"" << dt.icon_size << "\">";
            break;
        case IsDocument:
            if(icon != Image{}){
                h << Thumbnail{ icon.id, dt.icon_size };
            } else {
                std::string k   = get_value(icons, suffix);
                if(!k.empty()){
                    h << "<img src=\"" << k << "\" class=\"" << dt.icon_size << "\">";
                } else {
                    h << "<img src=\"/img/document.svg\" class=\"" << dt.icon_size << "\">";
                }
            }
            break;
        case IsLeaf:
            if(icon != Image{}){
                h << Thumbnail{ icon.id, dt.icon_size };
            } else {
                h << "<img src=\"/img/ext/y.svg\" class=\"" << dt.icon_size << "\">";
            }
            break;
        case None:
        default:
            if(icon != Image{}){
                h << Thumbnail{ icon.id, dt.icon_size };
            }
            break;
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    
    struct XSQ : public CacheQuery {
        std::string         sqldata;
        XSQ(std::string&& sql) : CacheQuery(sql), sqldata(std::move(sql)) 
        {
        }
    };
    
    
    struct XEntry::Tokens {
        XSQ     visible_skey;
        XSQ     hidden_skey;
    
        Tokens(std::string_view basis, std::string_view visible, std::string_view order_skey) : 
            visible_skey(concat({basis, visible, order_skey})), 
            hidden_skey(concat({basis, order_skey}))
        {
        }
        
        XSQ& operator[](const Details& dt) 
        {
            return dt.option.hidden ? hidden_skey : visible_skey;
        }
        
    };

    std::vector<XEntry>  XEntry::query(Folder x, const Details& dt)
    {
        std::vector<XEntry> ret;
        if(dt.query.folders)
            query_folders(ret, x, dt);
        if(dt.query.documents)
            query_documents(ret, x, dt);
        if(dt.query.leafs)
            query_leafs(ret, x, dt);
        if(dt.option.merge)
            std::stable_sort( ret.begin(), ret.end(), XEntry::isLess_skey);
        return ret;
    }
    

    size_t                XEntry::query_documents(std::vector<XEntry>&ret, Folder x, const Details& dt)
    {
        static thread_local Tokens sql("SELECT id,sk,name,icon,suffix,k FROM Documents WHERE folder=?", " and hidden=0", " ORDER BY sk");
        
        size_t      cnt = 0;
        CacheQuery& sd  = sql[dt];
        auto bf = sd.af();
        sd.bind(1, x.id);
        
        while(sd.step() == SQResult::Row){
            XEntry   ee;
            ee.document = { sd.v_uint64(1) };
            ee.skey     = sd.v_string(2);
            ee.name     = sd.v_string(3);
            ee.type     = IsDocument;
            if(ee.name.empty())
                ee.label    = ee.skey;
            else
                ee.label    = ee.name;
            ee.icon     = Image{ sd.v_uint64(4) };
            ee.suffix   = sd.v_string(5);
            ee.key      = sd.v_string(6);
            if(dt.option.root_dirs)
                ee.root_dirs    = cdb::root_dirs( ee.document );
            ret.push_back(ee);
            ++cnt;
        }

        return cnt;
    }

    size_t                XEntry::query_folders(std::vector<XEntry>&ret, Folder x, const Details& dt)
    {
        static thread_local Tokens  sql("SELECT id,sk,name,icon,k FROM Folders WHERE parent=?", " and hidden=0", " ORDER BY sk");
        size_t      cnt = 0;
        CacheQuery& sf  = sql[dt];
        auto af = sf.af();
        sf.bind(1, x.id);
        while(sf.step() == SQResult::Row){
            XEntry   ee;
            ee.folder   = { sf.v_uint64(1) };
            ee.type     = IsFolder;
            ee.skey     = sf.v_string(2);
            ee.name     = sf.v_string(3);
            if(ee.name.empty())
                ee.label    = ee.skey;
            else
                ee.label    = ee.name;
            ee.icon     = Image{ sf.v_uint64(4) };
            ee.key      = sf.v_string(5);
            if(dt.option.root_dirs)
                ee.root_dirs    = cdb::root_dirs( ee.folder );
            ret.push_back(ee);
            ++cnt;
        }
        return cnt;
    }
    
    
    size_t                XEntry::query_leafs(std::vector<XEntry>&ret, Folder x, const Details& dt)
    {
        static thread_local Tokens      sql("SELECT Leafs.id,Documents.skc,Leafs.title,Leafs.icon,Leafs.k FROM Leafs INNER JOIN Documents ON Leafs.id=Documents.id WHERE Documents.folder=?", " and Documents.hidden=0", " ORDER BY Documents.sk");
        size_t      cnt = 0;
        CacheQuery& sf  = sql[dt];
        auto af = sf.af();
        sf.bind(1, x.id);
        while(sf.step() == SQResult::Row){
            XEntry   ee;
            ee.leaf   = { sf.v_uint64(1) };
            ee.type     = IsLeaf;
            ee.skey     = sf.v_string(2);
            ee.title   = sf.v_string(3);
            if(ee.title.empty())
                ee.label    = ee.skey;
            else
                ee.label    = ee.title;
            ee.icon     = Image{ sf.v_uint64(4) };
            ee.key      = sf.v_string(5);
            if(dt.option.root_dirs)
                ee.root_dirs    = cdb::root_dirs( ee.document );
            ret.push_back(ee);
            ++cnt;
        }
        return cnt;
    }


}
