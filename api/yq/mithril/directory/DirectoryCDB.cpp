////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "DirectoryCDB.hpp"

#include <yq/mithril/folder/Folder.hpp>
#include <yq/mithril/folder/FolderCDB.hpp>
#include <yq/mithril/fragment/Fragment.hpp>
#include <yq/mithril/root/RootDir.hpp>
#include <yq/mithril/wksp/CacheQuery.hpp>
#include <yq/mithril/wksp/Workspace.hpp>
#include <yq/mithril/logging.hpp>
#include <yq/mithril/fragment/FragmentCDB.hpp>
#include <yq/text/copy.hpp>

namespace yq::mithril::cdb {
    std::vector<Directory>   all_directories(unsigned opts)
    {
        if(opts & BEST_SORT){
            if(opts & HIDDEN){
                static thread_local CacheQuery    s("SELECT id FROM " TBL_DIRECTORIES " ORDER BY path");
                return s.vec<Directory>();
            } else {
                static thread_local CacheQuery    s("SELECT id FROM " TBL_DIRECTORIES " WHERE hidden=0 ORDER BY path");
                return s.vec<Directory>();
            }
        } else {
            if(opts & HIDDEN){
                static thread_local CacheQuery    s("SELECT id FROM " TBL_DIRECTORIES "");
                return s.vec<Directory>();
            } else {
                static thread_local CacheQuery    s("SELECT id FROM " TBL_DIRECTORIES " WHERE hidden=0");
                return s.vec<Directory>();
            }
        }
    }
    
    std::vector<Directory>   all_directories(Sorted sorted)
    {
        return all_directories(HIDDEN | (sorted ? BEST_SORT : 0));
    }

    size_t              all_directories_count()
    {
        static thread_local CacheQuery    s("SELECT COUNT(1) FROM " TBL_DIRECTORIES "");
        return s.size();
    }
    
    DirOrFrag           child(Directory d, std::string_view ck)
    {
        Fragment    f   = child_fragment(d, ck);
        if(f)
            return f;
        Directory   dir = child_directory(d, ck);
        if(dir)
            return dir;
        return false;
    }

    size_t              child_count(Directory d)
    {
        return child_directories_count(d) + child_fragments_count(d);
    }

    Directory           child_directory(Directory d, std::string_view k)
    {
        static thread_local CacheQuery s("SELECT id FROM " TBL_DIRECTORIES " WHERE parent=? AND name=?");
        return s.as<Directory>(d.id, k);
    }

    Fragment            child_fragment(Directory d, std::string_view k)
    {
        static thread_local CacheQuery s("SELECT id FROM " TBL_FRAGMENTS " WHERE dir=? AND name=? LIMIT 1");
        return s.as<Fragment>(d.id, k);
    }

#if 0
    std::string                 child_key(Directory);       //<! Key inside the directory (with extensions)
#endif

    std::vector<Directory>   child_directories(Directory d, Sorted sorted)
    {
        static thread_local CacheQuery    qs("SELECT id FROM " TBL_DIRECTORIES " WHERE parent=? ORDER BY name");
        static thread_local CacheQuery    qu("SELECT id FROM " TBL_DIRECTORIES " WHERE parent=?");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Directory>(d.id);
    }
    
    size_t              child_directories_count(Directory d)
    {
        static thread_local CacheQuery    s("SELECT COUNT(1) FROM " TBL_DIRECTORIES " WHERE parent=?");
        return s.size(d.id);
    }
    
    Set<Directory>             child_directories_set(Directory d)
    {
        static thread_local CacheQuery    s("SELECT id FROM " TBL_DIRECTORIES " WHERE parent=?");
        return s.set<Directory>(d.id);
    }

    std::vector<DirString>   child_directories_with_names(Directory dir, Sorted sorted)
    {
        std::vector<DirString>   ret;
        static thread_local CacheQuery    qs("SELECT id,name FROM " TBL_DIRECTORIES " WHERE parent=? ORDER BY path");
        static thread_local CacheQuery    qu("SELECT id,name FROM " TBL_DIRECTORIES " WHERE parent=?");
        CacheQuery& s = sorted ? qs : qu;
        s.bind(1,dir.id);
        while(s.step() == SQResult::Row){
            ret.push_back(DirString(Directory(s.v_uint64(1)), s.v_text(1)));
        }
        s.reset();
        return ret;
    }

    std::vector<Fragment>    child_fragments(Directory d, Sorted sorted)
    {
        static thread_local CacheQuery    qs("SELECT id FROM " TBL_FRAGMENTS " WHERE dir=? ORDER BY path");
        static thread_local CacheQuery    qu("SELECT id FROM " TBL_FRAGMENTS " WHERE dir=?");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Fragment>(d.id);
    }

    size_t              child_fragments_count(Directory d)
    {
        static thread_local CacheQuery        s("SELECT COUNT(1) FROM " TBL_FRAGMENTS " WHERE dir=?");
        return s.size(d.id);
    }

    Set<Fragment>    child_fragments_set(Directory d)
    {
        static thread_local CacheQuery    s("SELECT id FROM " TBL_FRAGMENTS " WHERE dir=?");
        return s.set<Fragment>(d.id);
    }

    std::vector<DirOrFrag>   children(Directory d, Sorted sorted)
    {
        std::vector<DirOrFrag>   ret;
        for(Directory d2 : child_directories(d, sorted))
            ret.push_back(d2);
        for(Fragment f : child_fragments(d, sorted))
            ret.push_back(f);
        return ret;
    }
    
    std::vector<DirOrFragStr>    children_and_names(Directory);  // TODO

    std::pair<Directory, Folder>    db_directory(Directory dirParent, std::string_view k, bool *wasCreated)
    {
        if(wasCreated)
            *wasCreated = false;
        if(k.empty() || !dirParent)
            return {};
        const RootDir* rt  = root_dir(dirParent);
        if(!rt)
            return {};

        Folder f           = db_folder(folder(dirParent), k);
        if(!f)
            return {};
            
        std::string p       = path(dirParent);
        p += '/';
        p += k;
        
            
        static thread_local CacheQuery    i("INSERT OR FAIL INTO " TBL_DIRECTORIES " (path,root,folder,parent,name) VALUES (?,?,?,?,?)");
        static thread_local CacheQuery    s("SELECT id FROM " TBL_DIRECTORIES " WHERE path=?");
        auto s_lk   = s.af();
        auto i_lk   = i.af();

        i.bind(1, p);
        i.bind(2,rt->id);
        i.bind(3,f.id);
        i.bind(4,dirParent.id);
        i.bind(5,k);
        
        if(is_good(i.step())){
            if(wasCreated)
                *wasCreated = true;
            return { Directory((uint64_t) i.last_id()), f};
        } else {
            s.bind(1, p);
            if(s.step() == SQResult::Row)
                return { Directory(s.v_uint64(1)), f };
            cdbError << "Unable to get directory ID";
            return {};
        }
    }
    
    //Fragment                db_fragment(Document, const RootDir*); // TODO

    Directory           directory(uint64_t i)
    {
        return exists_directory(i) ? Directory{i} : Directory();
    }

    Directory           directory(const std::filesystem::path&p)
    {
        if(p.empty())
            return Directory();
        static thread_local CacheQuery    s("SELECT id FROM " TBL_DIRECTORIES " WHERE path=? LIMIT 1");
        return s.as<Directory>(p);
    }
    
    //std::vector<Document>    documents(Directory);

    void    erase(Directory d)
    {
        if(!d)
            return ;
        
        Folder          f   = folder(d);
        if(f.id && f.id <= kMaxFixedFolder)
            return ;
        
        {
            static thread_local CacheQuery dq("DELETE FROM " TBL_DIRECTORIES " WHERE id=?");
            dq.bind(1, d.id);
            dq.step();
            dq.reset();
        }
        
        if(!f)
            return ;
        
        size_t dc = directories_count(f);
        if(!dc){
            static thread_local CacheQuery dq("DELETE FROM " TBL_FOLDERS " WHERE id=?");
            dq.bind(1, f.id);
            dq.step();
            dq.reset();
        }
    }

    bool                exists(Directory d)
    {
        return exists_directory(d.id);
    }
    
    bool                exists_directory(uint64_t i)
    {
        static thread_local CacheQuery s("SELECT 1 FROM " TBL_DIRECTORIES " WHERE id=? LIMIT 1");
        return s.present(i);
    }

    Folder              folder(Directory d)
    {
        static thread_local CacheQuery    s("SELECT folder FROM " TBL_DIRECTORIES " WHERE id=?");
        return s.as<Folder>(d.id);
    }

    std::vector<Folder>      folder_path(Directory d)
    {
        return folder_path(folder(d));
    }


    bool                hidden(Directory d)
    {
        static thread_local CacheQuery    s("SELECT hidden FROM " TBL_DIRECTORIES " WHERE id=?");
        return s.boolean(d.id);
    }
    
    Directory::Info     info(Directory d)
    {
        Directory::Info        ret;
        static thread_local CacheQuery    s("SELECT folder, name, parent, path, removed, root, hidden FROM " TBL_DIRECTORIES " WHERE id=?");
        s.bind(1, d.id);
        if(s.step() == SQResult::Row){
            ret.folder  = Folder(s.v_uint64(1));
            ret.name    = s.v_text(2);
            ret.parent  = Directory(s.v_uint64(3));
            ret.path    = s.v_text(4);
            ret.removed = s.v_bool(5);
            ret.root_dir    = wksp::root_dir(s.v_uint64(6));
            ret.hidden  = s.v_bool(7);
        }
        s.reset();
        return ret;
    }

    std::string             key(Directory d)
    {
        return key(folder(d));
    }

    //std::string             label(Directory);

    std::string             name(Directory d)
    {
        static thread_local CacheQuery    s("SELECT name FROM " TBL_DIRECTORIES " WHERE id=?");
        return s.str(d.id);
    }
    
    Directory           parent(Directory d)
    {
        static thread_local CacheQuery    s("SELECT parent FROM " TBL_DIRECTORIES " WHERE id=?");
        return s.as<Directory>(d.id);
    }

    std::filesystem::path   path(Directory d)
    {
        static thread_local CacheQuery    s("SELECT path FROM " TBL_DIRECTORIES " WHERE id=?");
        return s.path(d.id);
    }
   
    bool                removed(Directory d)
    {
        static thread_local CacheQuery    s("SELECT removed FROM " TBL_DIRECTORIES " WHERE id=?");
        return s.boolean(d.id);
    }
    
    const RootDir*         root_dir(Directory d)
    {
        static thread_local CacheQuery    s("SELECT root FROM " TBL_DIRECTORIES " WHERE id=?");
        auto s_lk   = s.af();
        s.bind(1, d.id);
        if(s.step() == SQResult::Row)
            return wksp::root_dir(s.v_uint64(1));
        return nullptr;
    }
    
    std::string             skey(Directory d)
    {
        static thread_local CacheQuery    s("SELECT name FROM " TBL_DIRECTORIES " WHERE id=?");
        return s.str(d.id);
    }

    std::string             skeyb(Directory d)
    {
        std::string     bk  = skey(d);
        return copy(base_key(bk));
    }
}
