////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FolderCDB.hpp"
#include <io/DirUtils.hpp>
#include <basic/TextUtils.hpp>
#include <mithril/bit/NKI.hpp>
#include <mithril/directory/Directory.hpp>
#include <mithril/document/DocumentCDB.hpp>
#include <mithril/fragment/Fragment.hpp>
#include <mithril/root/RootDir.hpp>
#include <mithril/wksp/CacheQuery.hpp>
#include <mithril/wksp/Workspace.hpp>
#include <mithril/wksp/CacheLogging.hpp>

namespace yq::mithril::cdb {
    std::vector<Folder>      all_folders(Sorted sorted)
    {
        static thread_local CacheQuery    qs("SELECT id FROM Folders ORDER BY k");
        static thread_local CacheQuery    qu("SELECT id FROM Folders");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Folder>();
    }
    
    size_t              all_folders_count()
    {
        static thread_local CacheQuery s("SELECT COUNT(1) FROM Folders");
        return s.size();
    }

    std::string             brief(Folder f)
    {
        static thread_local CacheQuery    s("SELECT brief FROM Folders WHERE id=?");
        return s.str(f.id);
    }
    
    DocOrFold           child(Folder f, std::string_view ck)
    {
        Document    doc   = child_document(f, ck);
        if(doc)
            return doc;
        Folder      fold  = child_folder(f, ck);
        if(fold)
            return fold;
        return false;
    }

    size_t              child_count(Folder f)
    {
        return child_documents_count(f) + child_folders_count(f);
    }

    Document            child_document(Folder f, std::string_view k)
    {
        static thread_local CacheQuery s("SELECT id FROM Documents WHERE folder=? AND sk=?");
        return s.as<Document>(f.id, k);
    }

    Document        child_document(Folder f, uint64_t n, Sorted sorted)
    {
        return child_document(f, n, HIDDEN | (sorted ? BEST_SORT : 0));
    }

    Document        child_document(Folder f, uint64_t n, unsigned opts)
    {
        static thread_local CacheQuery  qsh("SELECT id FROM Documents WHERE folder=? ORDER BY sk LIMIT 1,?");
        static thread_local CacheQuery  qsv("SELECT id FROM Documents WHERE folder=? AND hidden=0 ORDER BY sk LIMIT 1,?");
        static thread_local CacheQuery  quh("SELECT id FROM Documents WHERE folder=? LIMIT 1,?");
        static thread_local CacheQuery  quv("SELECT id FROM Documents WHERE folder=? AND hidden=0 LIMIT 1,?");

        CacheQuery& s   = (opts & BEST_SORT) ? ((opts & HIDDEN) ? qsh : qsv) : ((opts & HIDDEN) ? quh : quv);
        auto af = s.af();
        
        s.bind(1, f);
        s.bind(2, n);
        
        if(s.step() == SQResult::Row)
            return Document(s.v_uint64(1));
        return Document();
    }

    std::vector<Document>    child_documents(Folder f, unsigned opts)
    {
        if(opts & BEST_SORT){
            if(opts & HIDDEN){
                static thread_local CacheQuery    s("SELECT id FROM Documents WHERE folder=? ORDER BY sk");
                return s.vec<Document>(f.id);
            } else {
                static thread_local CacheQuery    s("SELECT id FROM Documents WHERE folder=? AND hidden=0 ORDER BY sk");
                return s.vec<Document>(f.id);
            }
        } else {
            if(opts & HIDDEN){
                static thread_local CacheQuery    s("SELECT id FROM Documents WHERE folder=?");
                return s.vec<Document>(f.id);
            } else {
                static thread_local CacheQuery    s("SELECT id FROM Documents WHERE folder=? AND hidden=0");
                return s.vec<Document>(f.id);
            }
        }
    }
    
    std::vector<Document>    child_documents(Folder f, Sorted sorted)
    {
        return child_documents(f, HIDDEN | (sorted ? BEST_SORT : 0));
    }

    size_t              child_documents_count(Folder f, unsigned opts)
    {
        if(opts & HIDDEN){
            static thread_local CacheQuery    s("SELECT COUNT(1) FROM Documents WHERE folder=?");
            return s.size(f.id);
        } else {
            static thread_local CacheQuery    s("SELECT COUNT(1) FROM Documents WHERE folder=? AND hidden=0");
            return s.size(f.id);
        }
    }
    
    std::vector<Document>    child_documents_by_suffix(Folder f, std::string_view sfx, Sorted sorted)
    {
        static thread_local CacheQuery    qs("SELECT id FROM Documents WHERE folder=? AND suffix=? ORDER BY k");
        static thread_local CacheQuery    qu("SELECT id FROM Documents WHERE folder=? AND suffix=?");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Document>(f.id, sfx);
    }
    
    std::vector<Document>    child_documents_by_suffix_excluding(Folder f, std::string_view sfx, Sorted sorted)
    {
        static thread_local CacheQuery    qs("SELECT id FROM Documents WHERE folder!=? AND suffix=? ORDER BY k");
        static thread_local CacheQuery    qu("SELECT id FROM Documents WHERE folder!=? AND suffix=?");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Document>(f.id, sfx);
    }

    //! Gets all documents & keys of folder
    std::vector<DocString>          child_documents_with_skey(Folder f, unsigned opts)
    {
        static thread_local CacheQuery  s1("SELECT id, sk FROM Documents WHERE folder=? AND hidden=0");
        static thread_local CacheQuery  s2("SELECT id, sk FROM Documents WHERE folder=? AND hidden=0 ORDER BY sk");
        static thread_local CacheQuery  s3("SELECT id, sk FROM Documents WHERE folder=?");
        static thread_local CacheQuery  s4("SELECT id, sk FROM Documents WHERE folder=? ORDER BY sk");
        
        CacheQuery& s = (opts & HIDDEN) ? ((opts & BEST_SORT) ? s4 : s3 ) : ((opts & BEST_SORT) ? s2 : s1);
        auto s_af = s.af();
        std::vector<DocString>  ret;
        s.bind(1, f.id);
        while(s.step() == SQResult::Row)
            ret.push_back(DocString{ s.v_uint64(1), s.v_string(2) });
        return ret;
    }

    std::vector<DocString>          child_documents_with_skey(Folder f, Sorted sorted)
    {
        return child_documents_with_skey(f, HIDDEN | (sorted ? BEST_SORT : 0));
    }

    Folder              child_folder(Folder f, std::string_view ck)
    {
        static thread_local CacheQuery    s("SELECT id FROM Folders WHERE parent=? AND ck=? LIMIT 1");
        return s.as<Folder>(f.id, ck);
    }

    Folder                          child_folder(Folder f, uint64_t n, Sorted sorted)
    {
        return child_folder(f, n, HIDDEN | (sorted ? BEST_SORT : 0));
    }

    Folder                          child_folder(Folder f, uint64_t n, unsigned int opts)
    {
        static thread_local CacheQuery  qsh("SELECT id FROM Folders WHERE parent=? ORDER BY sk LIMIT 1,?");
        static thread_local CacheQuery  qsv("SELECT id FROM Folders WHERE parent=? AND hidden=0 ORDER BY sk LIMIT 1,?");
        static thread_local CacheQuery  quh("SELECT id FROM Folders WHERE parent=? LIMIT 1,?");
        static thread_local CacheQuery  quv("SELECT id FROM Folders WHERE parent=? AND hidden=0 LIMIT 1,?");

        CacheQuery& s   = (opts & BEST_SORT) ? ((opts & HIDDEN) ? qsh : qsv) : ((opts & HIDDEN) ? quh : quv);
        auto af = s.af();
        s.bind(1, f);
        s.bind(2, n);
        if(s.step() == SQResult::Row)
            return Folder(s.v_uint64(1));
        return Folder();
    }
    

    std::vector<Folder>      child_folders(Folder f, unsigned opts)
    {
        static thread_local CacheQuery    s1("SELECT id FROM Folders WHERE parent=? ORDER BY sk");
        static thread_local CacheQuery    s2("SELECT id FROM Folders WHERE parent=? AND hidden=0 ORDER BY sk");
        static thread_local CacheQuery    s3("SELECT id FROM Folders WHERE parent=?");
        static thread_local CacheQuery    s4("SELECT id FROM Folders WHERE parent=? AND hidden=0");
        
        CacheQuery& s   = (opts&HIDDEN)?((opts&BEST_SORT)?s1:s3):((opts&BEST_SORT)?s2:s4);
        return s.vec<Folder>(f.id);
    }

    std::vector<Folder>      child_folders(Folder f, Sorted sorted)
    {
        return child_folders(f, HIDDEN | (sorted ? BEST_SORT : 0));
    }
    
    
    size_t              child_folders_count(Folder f, unsigned opts)
    {
        if(opts & HIDDEN){
            static thread_local CacheQuery    s("SELECT COUNT(1) FROM Folders WHERE parent=?");
            return s.size(f.id);
        } else {
            static thread_local CacheQuery    s("SELECT COUNT(1) FROM Folders WHERE parent=? AND hidden=0");
            return s.size(f.id);
        }
    }

    std::vector<FolderStr>          child_folders_with_skey(Folder f, unsigned int opts)
    {
        static thread_local CacheQuery      s1("SELECT id, sk FROM Folders WHERE parent=? AND hidden=0 ORDER BY sk");
        static thread_local CacheQuery      s2("SELECT id, sk FROM Folders WHERE parent=? ORDER BY sk");
        static thread_local CacheQuery      s3("SELECT id, sk FROM Folders WHERE parent=? AND hidden=0");
        static thread_local CacheQuery      s4("SELECT id, sk FROM Folders WHERE parent=?");
        CacheQuery& s   = (opts&HIDDEN)?((opts&BEST_SORT)?s2:s4):((opts&BEST_SORT)?s1:s3);
        std::vector<FolderStr>      ret;
        auto   s_af = s.af();
        s.bind(1, f.id);
        while(s.step() == SQResult::Row)
            ret.push_back(FolderStr{ s.v_uint64(1), s.v_string(2) });
        return ret;
    }

    std::vector<FolderStr>          child_folders_with_skey(Folder f, Sorted sorted)
    {
        return child_folders_with_skey(f, HIDDEN | (sorted ? BEST_SORT : 0));
    }

    std::vector<Fragment>    child_fragments(Folder f, Sorted sorted)
    {
        static thread_local CacheQuery    qs("SELECT id FROM Fragments WHERE folder=? ORDER BY path");
        static thread_local CacheQuery    qu("SELECT id FROM Fragments WHERE folder=?");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Fragment>(f.id);
    }
    
    size_t              child_fragments_count(Folder f)
    {
        static thread_local CacheQuery        s("SELECT COUNT(1) FROM Fragments WHERE folder=?");
        return s.size(f.id);
    }

#if 0
    std::string                 child_key(Folder);
#endif

    
    Folder              db_folder(Folder f, std::string_view ck, bool *wasCreated)
    {
        if(wasCreated)
            *wasCreated = false;
        if(!f)
            return Folder{};
        if(ck.empty())
            return Folder{};

        std::string     k = key(f);
        if(k.empty()){
            k   = ck;
        } else {
            k += '/';
            k += ck;
        }
            
        static thread_local CacheQuery    i("INSERT OR FAIL INTO Folders (k,sk,name,parent,hidden) VALUES (?,?,?,?,?)");
        static thread_local CacheQuery    s("SELECT id FROM Folders WHERE k=?");

        auto s_lk   = s.af();
        auto i_lk   = i.af();
        
        
        bool    hidden  = (ck[0] == '.') || (ck.find_first_of('.') != std::string_view::npos);
        
        i.bind(1,k);
        i.bind(2,ck);
        i.bind(3,ck);
        i.bind(4,f.id);
        i.bind(5,hidden);
        
        if(is_good(i.step())){
            if(wasCreated)
                *wasCreated = true;
            return Folder{(uint64_t) i.last_id()};
        } else {
            s.bind(1,k);
            if(s.step() == SQResult::Row)
                return Folder{s.v_uint64(1)};
            cdbError << "Unable to get folder ID";
            return Folder{};
        }    
    }
    
    std::vector<Directory>   directories(Folder f, Sorted sorted)
    {
        static thread_local CacheQuery    qs("SELECT id FROM Directories WHERE folder=? ORDER BY name");
        static thread_local CacheQuery    qu("SELECT id FROM Directories WHERE folder=?");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Directory>(f.id);
    }
    
    size_t              directories_count(Folder f)
    {
        static thread_local CacheQuery    s("SELECT COUNT(1) FROM Directories WHERE folder=?");
        return s.size(f.id);
    }
    
    bool                exists(Folder f)
    {
        return exists_folder(f.id);
    }

    bool                exists(Folder fo, const RootDir*rt)
    {
        if(!rt)
            return false;
            
        static thread_local CacheQuery s("SELECT 1 FROM Directories WHERE folder=? AND root=? LIMIT 1");
        return s.present(fo.id, rt->id);
    }

    bool                exists_folder(uint64_t i)
    {
        static thread_local CacheQuery s("SELECT 1 FROM Folders WHERE id=? LIMIT 1");
        return s.present(i);
    }

    Directory           first_directory(Folder fo)
    {
        if(!fo)
            return Directory();
        static thread_local CacheQuery  s("SELECT id FROM Directories WHERE folder=? LIMIT 1");
        return s.as<Directory>(fo.id);
    }
    
    Directory           first_directory(Folder fo, const RootDir*r)
    {
        if(!r)
            return first_directory(fo);
        if(!fo)
            return Directory();
        
        static thread_local CacheQuery s("SELECT id FROM Directories WHERE folder=? AND root=? LIMIT 1");
        auto af = s.af();
        s.bind(1, fo.id);
        s.bind(2, r->id);
        if(s.step() == SQResult::Row)
            return Directory{ s.v_uint64(1)};
        return Directory{};
    }

    Document            first_document(Folder f, std::initializer_list<std::string_view> keys)
    {
        for(std::string_view k : keys){
            Document    d   = child_document(f, k);
            if(d)
                return d;
        }
        return Document{};
    }

    Document            first_document(Folder f, const std::vector<std::string>&keys)
    {
        for(const std::string& k : keys){
            Document    d   = child_document(f, k);
            if(d)
                return d;
        }
        return Document{};
    }

    Folder              folder(uint64_t i)
    {
        return exists_folder(i) ? Folder{i} : Folder{};
    }
    
    Folder              folder(std::string_view k)
    {
        static thread_local CacheQuery    s("SELECT id FROM Folders WHERE k=? LIMIT 1");
        return s.as<Folder>(k);
    }

    std::vector<Folder>      folder_path(Folder folder)
    {
        std::vector<Folder>  ret;
        for(Folder f = parent(folder); f; f = parent(f))
            ret.push_back(f);
        return std::vector<Folder>(ret.rbegin(), ret.rend());
    }
    
    bool                hidden(Folder f)
    {
        static thread_local CacheQuery    s("SELECT hidden FROM Folders WHERE id=?");
        return s.boolean(f.id);
    }
    
    Image               icon(Folder f)
    {
        static thread_local CacheQuery    s("SELECT icon FROM Folders WHERE id=? LIMIT 1");
        return s.as<Image>(f.id);
    }
    
    Folder::Info        info(Folder f)
    {
        Folder::Info        ret;
        static thread_local CacheQuery    s("SELECT k, sk, parent, name, brief, hidden, removed, icon FROM Folders WHERE id=?");
        s.bind(1,f.id);
        if(s.step() == SQResult::Row){
            ret.key         = s.v_text(1);
            ret.skey        = s.v_text(2);
            ret.parent      = Folder(s.v_uint64(3));
            ret.name        = s.v_text(4);
            ret.brief       = s.v_text(5);
            ret.hidden      = s.v_bool(6);
            ret.removed     = s.v_bool(7);
            ret.icon        = Image(s.v_uint64(8));
        }
        s.reset();
        return ret;
    }
    
    std::string             key(Folder f) 
    {
        static thread_local CacheQuery    s("SELECT k FROM Folders WHERE id=?");
        return s.str(f.id);
    }

    std::string             label(Folder f)
    {
        return key(f);
    }
    
    void    make_directory(Folder fo, const RootDir* rt)
    {
        if(!rt)
            return ;
        if(fo == top_folder())
            return ;
        make_path(rt->resolve(key(fo)));
    }

    std::string             name(Folder f)
    {
        static thread_local CacheQuery    s("SELECT name FROM Folders WHERE id=?");
        return s.str(f.id);
    }

    NKI                 nki(Folder f, bool autoKey)
    {
        static thread_local CacheQuery    s("SELECT name,icon,k FROM Folders WHERE id=?");
        auto s_af = s.af();
        s.bind(1, f.id);
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
    
    Folder              parent(Folder f)
    {
        static thread_local CacheQuery    s("SELECT parent FROM Folders WHERE id=?");
        return s.as<Folder>(f.id);
    }
    
    bool                removed(Folder f)
    {
        static thread_local CacheQuery    s("SELECT removed FROM Folders WHERE id=?");
        return s.boolean(f.id);
    }
    
    std::vector<const RootDir*> root_dirs(Folder f)
    {
        std::vector<const RootDir*> ret;
        static thread_local CacheQuery    s("SELECT DISTINCT root FROM Directories WHERE folder=?");
        auto s_af       = s.af();
        s.bind(1, f.id);
        while(s.step() == SQResult::Row){
            const RootDir*r    = wksp::root_dir(s.v_uint64(1));
            if(r)
                ret.push_back(r);
        }
        return ret;
    }
    
    size_t              roots_count(Folder f)
    {
        static thread_local CacheQuery    s("SELECT COUNT(DISTINCT root) FROM Directories WHERE folder=?");
        return s.size(f.id);
    }

    std::string             skey(Folder f)
    {
        static thread_local CacheQuery    s("SELECT sk FROM Folders WHERE id=?");
        return s.str(f.id);
    }
    
    std::string             skeyb(Folder f)
    {
        std::string     bk  = skey(f);
        return copy(base_key(bk));
    }
    

    bool                within(Folder p, Directory d, bool recursive)
    {
        return within(p, folder(d), recursive);
    }
    
    bool                within(Folder p, Document d, bool recursive)
    {
        if(!p || !d)    
            return false;
        Folder  c   = parent(d);
        if(p == c)
            return true;
        if(recursive){
            while((c = parent(c)))
                if(p == c)
                    return true;
        }
        return false;
    }
    
    bool                within(Folder p, Folder f, bool recursive)
    {
        if(!p || !f)    
            return false;
        Folder  c   = parent(f);
        if(p == c)
            return true;
        if(recursive){
            while((c = parent(c)))
                if(p == c)
                    return true;
        }
        return false;
    }
    
    bool                within(Folder p, Fragment f, bool recursive)
    {
        return within(p, document(f), true);
    }
}
