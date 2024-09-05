////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FragmentCDB.hpp"
#include <yq-toolbox/container/ByteArray.hpp>
#include <yq-toolbox/io/FileUtils.hpp>

#include <mithril/directory/DirectoryCDB.hpp>
#include <mithril/document/DocumentCDB.hpp>
#include <mithril/root/RootDir.hpp>
#include <mithril/wksp/CacheQuery.hpp>
#include <mithril/wksp/Workspace.hpp>
#include <mithril/wksp/CacheLogging.hpp>

namespace yq::mithril::cdb {
    FragmentVector   all_fragments(Sorted sorted)
    {
        static thread_local CacheQuery qs("SELECT id FROM " TBL_FRAGMENTS " ORDER BY path");
        static thread_local CacheQuery qu("SELECT id FROM " TBL_FRAGMENTS "");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Fragment>();
    }
    
    size_t              all_fragments_count()
    {
        static thread_local CacheQuery s("SELECT COUNT(1) FROM " TBL_FRAGMENTS "");
        return s.size();
    }
    
    FragmentVector   all_fragments_suffix(std::string_view sfx, Sorted sorted)
    {
        static thread_local CacheQuery qs("SELECT id FROM " TBL_FRAGMENTS " WHERE suffix=? ORDER BY path");
        static thread_local CacheQuery qu("SELECT id FROM " TBL_FRAGMENTS " WHERE suffix=?");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Fragment>(sfx);
    }


    std::string             base_key(Fragment f)
    {
        return base_key(document(f));
    }

#if 0
    std::string                 child_key(Fragment);
#endif
    
    
    std::vector<uint8_t>         data(Fragment);     // TODO

    std::pair<Fragment, Document>   db_fragment(Directory dirParent, std::string_view k, bool *wasCreated)
    {
        if(wasCreated)
            *wasCreated = false;
        if(k.empty() || is_space(k) || !dirParent)
            return {};

        Folder          f   = folder(dirParent);
        Document        a   = db_document(f, k);
        std::filesystem::path         p   = path(dirParent) / k;
        std::string         sfx = suffix(a).ext;
        const RootDir*     rt  = root_dir(dirParent);
        
        
        static thread_local CacheQuery    i("INSERT OR FAIL INTO " TBL_FRAGMENTS " (path,name,dir,root,document,folder,suffix) VALUES (?,?,?,?,?,?,?)");
        static thread_local CacheQuery    s("SELECT id FROM " TBL_FRAGMENTS " WHERE path=?");
        auto s_lk   = s.af();
        auto i_lk   = i.af();
        
        i.bind(1,p);
        i.bind(2,k);
        i.bind(3,dirParent.id);
        i.bind(4,rt->id);
        i.bind(5,a.id);
        i.bind(6,f.id);
        i.bind(7,sfx);
        
        if(is_good(i.step())){
            if(wasCreated)
                *wasCreated = true;
            return { Fragment((uint64_t) i.last_id()), a };
        } else {
            s.bind(1,p);
            if(s.step() == SQResult::Row)
                return { Fragment(s.v_uint64(1)), a };
            cdbError << "Unable to get fragment ID";
            return {};
        }
    }
    
    Directory           directory(Fragment f)
    {
        static thread_local CacheQuery    s("SELECT dir FROM " TBL_FRAGMENTS " WHERE id=?");
        return s.as<Directory>(f.id);
    }

    Document            document(Fragment f)
    {
        static thread_local CacheQuery    s("SELECT document FROM " TBL_FRAGMENTS " WHERE id=?");
        return s.as<Document>(f.id);
    }

    void    erase(Fragment f)
    {
        if(!f)
            return ;
            
        Document    doc = document(f);
        {
            static thread_local CacheQuery d("DELETE FROM " TBL_FRAGMENTS " WHERE id=?");
            d.bind(1, f.id);
            d.step();
            d.reset();
        }
        
        if(!doc)
            return ;
        
        size_t  fc  = fragments_count(doc);
        if(!fc){
            static thread_local CacheQuery d("DELETE FROM " TBL_DOCUMENTS " WHERE id=?");
            d.bind(1, doc.id);
            d.step();
            d.reset();
        }
    }
    
    bool                exists(Fragment f)
    {
        return exists_fragment(f.id);
    }

    bool                exists_fragment(uint64_t i)
    {
        static thread_local CacheQuery s("SELECT 1 FROM " TBL_FRAGMENTS " WHERE id=? LIMIT 1");
        return s.present(i);
    }
    
    Folder              folder(Fragment f)
    {
        static thread_local CacheQuery    s("SELECT folder FROM " TBL_FRAGMENTS " WHERE id=?");
        return s.as<Folder>(f.id);
    }

    FolderVector      folder_path(Fragment f)
    {
        return folder_path(document(f));
    }
    
    ByteArray           frag_bytes(Fragment f, cdb_options_t opts)
    {
        std::filesystem::path   p = path(f);
        Id::Lock  lk;
        if(!(opts & DONT_LOCK)){
            lk = Id(f).lock(false);
            if(!lk){
                cdbWarning << "Unable to get read lock on fragment: " << p;
                return ByteArray();
            }
        }
            
        return file_bytes(p);
    }
    
    ByteArray           frag_bytes(std::string_view k, cdb_options_t opts)
    {
        return frag_bytes(fragment(document(k)), opts);
    }
    
    ByteArray           frag_bytes(std::string_view k, DataRole dr, cdb_options_t opts)
    {
        return frag_bytes(fragment(document(k),dr), opts);
    }
    
    
    std::string              frag_string(Fragment f, cdb_options_t opts)
    {
        std::filesystem::path   p = path(f);
        Id::Lock  lk;
        if(!(opts & DONT_LOCK)){
            lk = Id(f).lock(false);
            if(!lk){
                cdbWarning << "Unable to get read lock on fragment: " << p;
                return std::string();
            }
        }
            
        return file_string(p);
    }
    
    std::string              frag_string(std::string_view k, cdb_options_t opts)
    {
        return frag_string(fragment(document(k)), opts);
    }
    
    std::string              frag_string(std::string_view k, DataRole dr, cdb_options_t opts)
    {
        return frag_string(fragment(document(k),dr), opts);
    }

    Fragment            fragment(const std::filesystem::path&k)
    {
        static thread_local CacheQuery   s("SELECT id FROM " TBL_FRAGMENTS " WHERE path=? LIMIT 1");
        return s.as<Fragment>(k);
    }

    Fragment            fragment(std::string_view k)
    {
        static thread_local CacheQuery    s("SELECT id FROM " TBL_FRAGMENTS " WHERE path=? LIMIT 1");
        return s.as<Fragment>(k);
    }
    
    Fragment            fragment(uint64_t i)
    {
        return exists_fragment(i) ? Fragment{i} : Fragment{};
    }
    
    
    FragmentVector      fragments(std::string_view k, Sorted sorted)
    {
        return fragments(document(k),sorted);
    }
    
    FragmentVector      fragments(std::string_view k, DataRole dr, Sorted sorted)
    {
        return fragments(document(k), dr, sorted);
    }

    bool                hidden(Fragment f)
    {
        static thread_local CacheQuery    s("SELECT hidden FROM " TBL_FRAGMENTS " WHERE id=?");
        return s.boolean(f.id);
    }
    
    Fragment::Info      info(Fragment f)
    {
        Fragment::Info        ret;

        static thread_local CacheQuery    s("SELECT document, dir, folder, modified, name, path, removed, rescan, bytes, hidden, root FROM " TBL_FRAGMENTS " WHERE id=?");
        s.bind(1, f.id);
        if(s.step() == SQResult::Row){
            ret.document    = Document(s.v_uint64(1));
            ret.directory   = Directory(s.v_uint64(2));
            ret.folder      = Folder(s.v_uint64(3));
            ret.modified    = s.v_uint64(4);
            ret.name        = s.v_text(5);
            ret.path        = s.v_text(6);
            ret.removed     = s.v_bool(7);
            ret.rescan      = s.v_bool(8);
            ret.size        = s.v_uint64(9);
            ret.hidden      = s.v_bool(10);
            ret.root_dir        = wksp::root_dir(s.v_uint64(11));
        }
        s.reset();
        return ret;
    }
    
    std::string             key(Fragment f)
    {
        return key(document(f));
    }
    
    std::string             label(Fragment f)
    {
        return path(f).string();
    }

    uint64_t                modified(Fragment f)
    {
        static thread_local CacheQuery    s("SELECT modified FROM " TBL_FRAGMENTS " WHERE id=?");
        return s.u64(f.id);
    }
    
    std::string             name(Fragment f)
    {
        static thread_local CacheQuery    s("SELECT name FROM " TBL_FRAGMENTS " WHERE id=?");
        return s.str(f.id);
    }
    
    Directory           parent(Fragment f)
    {
        return directory(f);
    }

    std::filesystem::path   path(Fragment f)
    {
        static thread_local CacheQuery    s("SELECT path FROM " TBL_FRAGMENTS " WHERE id=?");
        return s.path(f.id);
    }
    
    bool                removed(Fragment f)
    {
        static thread_local CacheQuery    s("SELECT removed FROM " TBL_FRAGMENTS " WHERE id=?");
        return s.boolean(f.id);
    }
    
    void                rescan(Fragment f)
    {
        static thread_local CacheQuery    u("UPDATE " TBL_FRAGMENTS " SET rescan=1 WHERE id=?");
        u.bind(1, f.id);
        u.step();
        u.reset();
    }
    
    bool                rescanning(Fragment f)
    {
        static thread_local CacheQuery    s("SELECT rescan FROM " TBL_FRAGMENTS " WHERE id=?");
        return s.boolean(f.id);
    }

    const RootDir*         root_dir(Fragment f)
    {
        static thread_local CacheQuery    s("SELECT root FROM " TBL_FRAGMENTS " WHERE id=?");
        auto s_af   = s.af();
        s.bind(1, f.id);
        if(s.step() == SQResult::Row)
            return wksp::root_dir(s.v_uint64(1));
        return nullptr;
    }

    size_t              size(Fragment f)
    {
        static thread_local CacheQuery    s("SELECT bytes FROM " TBL_FRAGMENTS " WHERE id=?");
        return s.size(f.id);
    }
    
    std::string             skey(Fragment f)
    {
        static thread_local CacheQuery    s("SELECT name FROM " TBL_FRAGMENTS " WHERE id=?");
        return s.str(f.id);
    }
    
    std::string             skeyb(Fragment f)
    {
        std::string     bk  = skey(f);
        return copy(base_key(bk));
    }

    Extension              suffix(Fragment f)
    {
        static thread_local CacheQuery    s("SELECT suffix FROM " TBL_FRAGMENTS " WHERE id=?");
        return { s.str(f.id) };
    }

    void                update(Fragment f)
    {
        std::filesystem::path   p           = path(f);
        SizeTimestamp   sz  = file_size_and_timestamp(p.c_str());
    
        static thread_local CacheQuery    u("UPDATE " TBL_FRAGMENTS " SET bytes=?,modified=?,removed=?,rescan=0 WHERE id=?");
        u.bind(1, sz.size);
        u.bind(2, sz.nanoseconds());
        u.bind(3, !sz.exists);
        u.bind(4, f.id);
        u.step(true);
        u.reset();
    }
}
