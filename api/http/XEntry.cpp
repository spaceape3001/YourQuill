////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "XEntry.hpp"
#include <basic/TextUtils.hpp>
#include <kernel/file/DocumentCDB.hpp>
#include <kernel/file/FolderCDB.hpp>
#include <kernel/leaf/LeafCDB.hpp>
#include <kernel/wksp/CacheQuery.hpp>

namespace yq {

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
    
    XEntry::XEntry(const XEntry&) = default;
    XEntry::XEntry(XEntry&&) = default;
    XEntry::~XEntry() = default;
    XEntry& XEntry::operator=(const XEntry&) = default;
    XEntry& XEntry::operator=(XEntry&&) = default;

    //XEntry::XEntry(Leaf);
    //XEntry::XEntry(Document);
    //XEntry::XEntry(Folder);

    ////////////////////////////////////////////////////////////////////////////////

    size_t                XEntry::query_documents(std::vector<XEntry>&ret, Folder x, unsigned int opts)
    {
        static thread_local CacheQuery  sd1("SELECT id,sk,name,icon,suffix FROM Documents WHERE folder=? and hidden=0 ORDER BY sk");
        static thread_local CacheQuery  sd2("SELECT id,sk,name,icon,suffix FROM Documents WHERE folder=? ORDER BY sk");

        size_t      cnt = 0;
        CacheQuery& sd  = (opts&cdb::HIDDEN) ? sd2 : sd1;
        auto bf = sd.af();
        sd.bind(1, x.id);
        
        while(sd.step() == SQResult::Row){
            XEntry   ee;
            ee.doc      = { sd.v_uint64(1) };
            ee.skey     = sd.v_string(2);
            ee.name     = sd.v_string(3);
            ee.type     = IsDocument;
            if(ee.name.empty())
                ee.label    = ee.skey;
            else
                ee.label    = ee.name;
            ee.icon     = Image{ sd.v_uint64(4) };
            ee.suffix   = sd.v_string(5);
            if(opts & cdb::ROOTS)
                ee.roots    = cdb::roots( ee.doc );
            ret.push_back(ee);
            ++cnt;
        }

        return cnt;
    }

    std::vector<XEntry>   XEntry::query_documents(Folder x, unsigned int opts)
    {
        std::vector<XEntry> ret;
        query_documents(ret, x, opts);
        return ret;
    }
    
    size_t                XEntry::query_folders(std::vector<XEntry>&ret, Folder x, unsigned int opts)
    {
        static thread_local CacheQuery  sf1("SELECT id,sk,name,icon FROM Folders WHERE parent=? and hidden=0 ORDER BY sk");
        static thread_local CacheQuery  sf2("SELECT id,sk,name,icon FROM Folders WHERE parent=? ORDER BY sk");
        size_t      cnt = 0;
        CacheQuery& sf  = (opts&cdb::HIDDEN) ? sf2 : sf1;
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
            if(opts & cdb::ROOTS)
                ee.roots    = cdb::roots( ee.folder );
            ret.push_back(ee);
            ++cnt;
        }
        return cnt;
    }
    

    std::vector<XEntry>   XEntry::query_folders(Folder x, unsigned int opts)
    {
        std::vector<XEntry> ret;
        query_folders(ret, x, opts);
        return ret;
    }
    
    size_t                XEntry::query_leafs(std::vector<XEntry>&ret, Folder x, unsigned int opts)
    {
        static thread_local CacheQuery  sl1("SELECT id,Documents.sk,title,icon FROM Leafs INNER JOIN ON Leafs.id=Documents.id WHERE Documents.parent=? and Documents.hidden=0ORDER BY Documents.sk");
        static thread_local CacheQuery  sl2("SELECT id,Documents.sk,title,icon FROM Leafs INNER JOIN ON Leafs.id=Documents.id WHERE Documents.parent=? ORDER BY Documents.sk");
        size_t      cnt = 0;
        CacheQuery& sf  = (opts&cdb::HIDDEN) ? sl2 : sl1;
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
            if(opts & cdb::ROOTS)
                ee.roots    = cdb::roots( ee.doc );
            ret.push_back(ee);
            ++cnt;
        }
        return cnt;
    }

    std::vector<XEntry>   XEntry::query_leafs(Folder x, unsigned int opts)
    {
        std::vector<XEntry> ret;
        query_leafs(ret, x, opts);
        return ret;
    }


}
