////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DocumentCDB.hpp"

#include <0/basic/CollectionUtils.hpp>
#include <0/basic/TextUtils.hpp>
#include <mithril/bit/NKI.hpp>
#include <mithril/folder/FolderCDB.hpp>
#include <mithril/fragment/Fragment.hpp>
#include <mithril/root/RootDir.hpp>
#include <mithril/wksp/Workspace.hpp>
#include <mithril/wksp/CacheQuery.hpp>
#include <mithril/wksp/CacheLogging.hpp>

namespace yq::mithril::cdb {
    std::vector<Document>    all_documents(Sorted sorted)
    {
        static thread_local CacheQuery    qs("SELECT id FROM Documents ORDER BY k");
        static thread_local CacheQuery    qu("SELECT id FROM Documents");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Document>();
    }
    
    size_t              all_documents_count()
    {
        static thread_local CacheQuery s("SELECT COUNT(1) FROM Documents");
        return s.size();
    }
    
    std::vector<Document>    all_documents_suffix(std::string_view sfx, Sorted sorted)
    {
        static thread_local CacheQuery    qs("SELECT id FROM Documents WHERE suffix=? ORDER BY k");
        static thread_local CacheQuery    qu("SELECT id FROM Documents WHERE suffix=?");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Document>(sfx);
    }

    std::string             base_key(Document doc)
    {
        static thread_local CacheQuery    s("SELECT skb FROM Documents WHERE id=?");
        return s.str(doc.id);
    }


#if 0
    std::string                 child_key(Document);
#endif

    Document            db_document(Folder f, std::string_view ak, bool *wasCreated)
    {
        if(wasCreated)
            *wasCreated = false;
        if(!f)
            return Document();
        if(ak.empty() || is_space(ak))
            return Document();

        std::string     k   = key(f);
        if(k.empty()){
            k   = ak;
        } else {
            k   += '/';
            k   += ak;
        }
                
        size_t  x = ak.find_last_of('.');
        size_t  y = ak.find_first_of('.',1);    // first period past any "hidden"
        
        std::string_view     sfx     = (x != std::string_view::npos ) ? ak.substr(x+1) : std::string_view();
        std::string_view     base    = (y != std::string_view::npos ) ? ak.substr(0,y) : ak;
        std::string_view     skc     = (x != std::string_view::npos ) ? ak.substr(0,x) : ak;
        
        static thread_local CacheQuery    i("INSERT OR FAIL INTO Documents (k,sk,name,folder,suffix,skb,hidden,mime,skc) VALUES (?,?,?,?,?,?,?,?,?)");
        static thread_local CacheQuery    s("SELECT id FROM Documents WHERE k=?");
        
        auto s_lk   = s.af();
        auto i_lk   = i.af();

        i.bind(1,k);
        i.bind(2,ak);
        i.bind(3,ak);
        i.bind(4,f.id);
        i.bind(5,sfx);
        i.bind(6,base);
        i.bind(7,ak[0] == '.');
        i.bind(8,mimeTypeForExt(sfx).value());
        i.bind(9,skc);
        
        if(is_good(i.step(false))){
            if(wasCreated)
                *wasCreated = true;
            Document    ret{(uint64_t) i.last_id()};
            // separate because I occasionally need to print during debugging
            return ret;
        } else {
            s.bind(1,k);
            if(s.step() == SQResult::Row){
                Document    ret{s.v_uint64(1)};
                // separate because I occasionally need to print during debugging
                return ret;
            }
            cdbError << "Unable to get document ID";
            return Document();
        }

    }

    Document            document(std::string_view k)
    {
        static thread_local CacheQuery    s("SELECT id FROM Documents WHERE k=? LIMIT 1");
        return s.as<Document>(k);
    }
    
    Document            document(uint64_t i)
    {
        return exists_document(i) ? Document{i} : Document();
    }

    bool                exists(Document d)
    {
        return exists_document(d.id);
    }

    bool                exists(Document doc, const RootDir* rt)
    {
        if(!rt)
            return false;
        static thread_local CacheQuery s("SELECT 1 FROM Fragments WHERE document=? AND root=? LIMIT 1");
        return s.present(doc.id, rt->id);
    }

    bool                exists_document(uint64_t i)
    {
        static thread_local CacheQuery s("SELECT 1 FROM Documents WHERE id=? LIMIT 1");
        return s.present(i);
    }

    Document            first_document(std::initializer_list<std::string_view> keys)
    {
        for(std::string_view k : keys){
            Document    d   = document(k);
            if(d)
                return d;
        }
        return Document{};
    }

    Document            first_document(const std::vector<std::string>& keys)
    {
        for(std::string_view k : keys){
            Document    d   = document(k);
            if(d)
                return d;
        }
        return Document{};
    }

    Fragment            first_fragment(Document d)
    {
        static thread_local CacheQuery s("SELECT id FROM Fragments WHERE document=? ORDER BY root LIMIT 1");
        return s.as<Fragment>(d.id);
    }
    
    Fragment            first_fragment(Document d, const RootDir*rt)
    {
        if(!rt)
            return first_fragment(d);
            
        static thread_local CacheQuery s("SELECT id FROM Fragments WHERE document=? AND root=? LIMIT 1");
        return s.as<Fragment>(d.id, rt->id);
    }

    Fragment            first_fragment(Document d, DataRole dr)
    {
        for(const RootDir* rt : wksp::root_reads()[dr]){
            Fragment f = first_fragment(d,rt);
            if(f)
                return f;
        }
        return Fragment{};
    }
    
    Folder              folder(Document d)
    {
        static thread_local CacheQuery    s("SELECT folder FROM Documents WHERE id=? LIMIT 1");
        return s.as<Folder>(d.id);
    }

    std::vector<Folder>      folder_path(Document doc)
    {
        std::vector<Folder>  ret;
        for(Folder f = parent(doc); f; f = parent(f))
            ret.push_back(f);
        return std::vector<Folder>(ret.rbegin(), ret.rend());
    }
    
    Fragment            fragment(Document d)
    {
        static thread_local CacheQuery    s("SELECT id FROM Fragments WHERE document=? LIMIT 1");
        return s.as<Fragment>(d.id);
    }
    
    Fragment            fragment(Document d, const RootDir* rt)
    {
        if(!rt)
            return Fragment{};
        static thread_local CacheQuery    s("SELECT id FROM Fragments WHERE document=? AND root=? LIMIT 1");
        return s.as<Fragment>(d.id, rt->id);
    }
    
    Fragment            fragment(Document d, DataRole dr)
    {
        for(const RootDir* r : wksp::root_reads(dr)){
            Fragment    f   = fragment(d, r);
            if(f)
                return f;
        }
        return Fragment();
    }

    namespace {
        std::vector<Fragment>    fragments_sorted(Document d)
        {
            static thread_local CacheQuery    s("SELECT id FROM Fragments WHERE document=? ORDER BY path");
            return s.vec<Fragment>(d.id);
        }

        std::vector<Fragment>    fragments_unsorted(Document d)
        {
            static thread_local CacheQuery    s("SELECT id FROM Fragments WHERE document=?");
            return s.vec<Fragment>(d.id);
        }
    }

    std::vector<Fragment>    fragments(Document d, Sorted sorted)
    {
        return sorted ? fragments_sorted(d) : fragments_unsorted(d);
    }
    
    std::vector<Fragment>        fragments(Document d, Sorted::Value sorted)
    {
        return sorted ? fragments_sorted(d) : fragments_unsorted(d);
    }
    
    std::vector<Fragment>    fragments(Document d, const RootDir* rt, Sorted sorted)
    {
        if(!rt)
            return std::vector<Fragment>();
        static thread_local CacheQuery    qs("SELECT id FROM Fragments WHERE document=? AND root=? ORDER BY path");
        static thread_local CacheQuery    qu("SELECT id FROM Fragments WHERE document=? AND root=?");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Fragment>(d.id, rt->id);
    }
    
    std::vector<Fragment>    fragments(Document d, const RootDir* rt, Sorted::Value sorted)
    {
        return fragments(d, rt, Sorted{sorted});
    }

    std::vector<Fragment>    fragments(Document d, DataRole dr, Sorted sorted)
    {
        std::vector<Fragment>    ret;
        for(const RootDir* rt : wksp::root_reads()[dr])
            ret += fragments(d,rt,sorted);
        return ret;
    }
    
    size_t              fragments_count(Document d)
    {
        static thread_local CacheQuery    s("SELECT COUNT(1) FROM Fragments WHERE document=?");
        return s.size(d.id);
    }
    
    size_t              fragments_count(Document d, const RootDir*rt)
    {
        if(!rt)
            return 0;
            
        static thread_local CacheQuery    s("SELECT COUNT(1) FROM Fragments WHERE document=? AND root=?");
        return s.size(d.id, rt->id);
    }

    bool                hidden(Document d)
    {
        static thread_local CacheQuery    s("SELECT hidden FROM Documents WHERE id=? LIMIT 1");
        return s.boolean(d.id);
    }
    
    void                hide(Document d)
    {
        static thread_local CacheQuery u("UPDATE Documents SET hidden=1 WHERE id=?");
        u.bind(1, d.id);
        u.exec();
        u.reset();
    }
    
    Image               icon(Document d) 
    {
        static thread_local CacheQuery    s("SELECT icon FROM Documents WHERE id=? LIMIT 1");
        return s.as<Image>(d.id);
    }
    
    Document::Info      info(Document d)
    {
        Document::Info        ret;
        static thread_local CacheQuery    s("SELECT k, sk, name, skb, folder, suffix, removed, hidden, icon, skc FROM Documents WHERE id=?");
        s.bind(1, d.id);
        if(s.step() == SQResult::Row){
            ret.key     = s.v_text(1);
            ret.skey    = s.v_text(2);
            ret.name    = s.v_text(3);
            ret.skeyb   = s.v_text(4);
            ret.folder  = Folder(s.v_uint64(5));
            ret.suffix  = s.v_text(6);
            ret.removed = s.v_bool(7);
            ret.hidden  = s.v_bool(8);
            ret.icon    = Image(s.v_uint64(9));
            ret.skeyc   = s.v_text(10);
        }
        s.reset();
        return ret;
    }
    
    std::string             key(Document d)
    {
        static thread_local CacheQuery    s("SELECT k FROM Documents WHERE id=? LIMIT 1");
        return s.str(d.id);
    }

    std::string             label(Document d)
    {
        return key(d);
    }

    ContentType             mime_type(Document d)
    {
        static thread_local CacheQuery  s("SELECT mime FROM Documents WHERE id=? LIMIT 1");
        auto af = s.af();
        s.bind(1, d.id);
        if(s.step() == SQResult::Row){
            return (ContentType::enum_t) s.v_int(1);
        } else 
            return ContentType();
    }
    
    std::string             name(Document d)
    {
        static thread_local CacheQuery    s("SELECT name FROM Documents WHERE id=? LIMIT 1");
        return s.str(d.id);
    }

    NKI                 nki(Document d, bool autoKey)
    {
        static thread_local CacheQuery    s("SELECT name,icon,k FROM Documents WHERE id=?");
        auto s_af = s.af();
        s.bind(1, d.id);
        if(s.step() == SQResult::Row){
            NKI  ret;
            ret.name    = s.v_text(1);
            ret.icon    = Image(s.v_uint64(2)) ;
            ret.key     = s.v_text(3);
            if(autoKey && ret.name.empty())
                ret.name    = ret.key;
            return ret;
        }
        return NKI{};
    }
    
    Folder              parent(Document d)
    {
        return folder(d);
    }
    
    bool                removed(Document d)
    {
        static thread_local CacheQuery    s("SELECT removed FROM Documents WHERE id=?");
        return s.boolean(d.id);
    }

    std::vector<const RootDir*> root_dirs(Document d)
    {
        std::vector<const RootDir*> ret;
        static thread_local CacheQuery    s("SELECT DISTINCT root FROM Fragments WHERE document=?");
        auto s_af       = s.af();
        s.bind(1, d.id);
        while(s.step() == SQResult::Row){
            const RootDir*r    = wksp::root_dir(s.v_uint64(1));
            if(r)
                ret.push_back(r);
        }
        return ret;
    }

    size_t              roots_count(Document d)
    {
        static thread_local CacheQuery    s("SELECT COUNT(DISTINCT root) FROM Fragments WHERE document=?");
        return s.size(d.id);
    }

    void                set_mime_type(Document doc, ContentType ct)
    {
        static thread_local CacheQuery  u("UPDATE Documents SET mime=? WHERE id=?");
        u.bind(1, ct.value());
        u.bind(2, doc.id);
        u.step(true);
        u.reset();
    }
    
    void                show(Document d)
    {
        static thread_local CacheQuery u("UPDATE Documents SET hidden=0 WHERE id=?");
        u.bind(1, d.id);
        u.step(true);
        u.reset();
    }
    
    std::string             skey(Document d)
    {
        static thread_local CacheQuery    s("SELECT sk FROM Documents WHERE id=?");
        return s.str(d.id);
    }
    
    std::string             skeyb(Document d)
    {
        static thread_local CacheQuery    s("SELECT skb FROM Documents WHERE id=?");
        return s.str(d.id);
    }

    std::string             skeyc(Document d)
    {
        static thread_local CacheQuery    s("SELECT skc FROM Documents WHERE id=?");
        return s.str(d.id);
    }

    Extension               suffix(Document d)
    {
        static thread_local CacheQuery    s("SELECT suffix FROM Documents WHERE id=?");
        return { s.str(d.id) };
    }

    Fragment            writable(Document d, DataRole dr)
    {
        for(const RootDir* rt : wksp::root_writes()[dr]){
            Fragment f = first_fragment(d, rt);
            if(f)
                return f;
        }
        return Fragment{};
    }
}
