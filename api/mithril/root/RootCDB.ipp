////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RootCDB.hpp"
#include <io/DirUtils.hpp>
#include <mithril/directory/DirectoryCDB.hpp>
#include <mithril/document/DocumentCDB.hpp>
#include <mithril/fragment/FragmentCDB.hpp>
#include <mithril/root/Root.hpp>
#include <mithril/wksp/CacheQuery.hpp>
#include <mithril/wksp/Workspace.hpp>
#include <mithril/wksp/CacheLogging.hpp>

namespace yq::mithril::cdb {
    std::vector<Directory>   all_directories(const Root*rt, Sorted sorted)
    {
        if(!rt)
            return std::vector<Directory>();

        static thread_local CacheQuery    qs("SELECT id FROM Directories WHERE root=? ORDER BY path");
        static thread_local CacheQuery    qu("SELECT id FROM Directories WHERE root=?");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Directory>(rt->id);
    }
    
    size_t              all_directories_count(const Root*rt)
    {
        if(!rt)
            return 0;

        static thread_local CacheQuery    s("SELECT COUNT(1) FROM Directories WHERE root=?");
        return s.size(rt->id);
    }
    
    std::vector<Fragment>    all_fragments(const Root*rt, Sorted sorted)
    {
        if(!rt)
            return std::vector<Fragment>();
        static thread_local CacheQuery qs("SELECT id FROM Fragments WHERE root=? ORDER BY path");
        static thread_local CacheQuery qu("SELECT id FROM Fragments WHERE root=?");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Fragment>(rt->id);
    }
    
    size_t              all_fragments_count(const Root*rt)
    {
        if(!rt)
            return 0;
        static thread_local CacheQuery s("SELECT COUNT(1) FROM Fragments WHERE root=?");
        return s.size(rt->id);
    }

    size_t                      all_roots_count()
    {
        return wksp::roots().size();
    }
    
    std::vector<DirOrFrag>   children(const Root* rt, Sorted sorted)
    {
        return children(directory(rt), sorted);
    }
    
    std::vector<uint8_t>         data(Fragment);     // TODO
    
    Directory           db_root(const Root*rt, bool *wasCreated)
    {
        if(wasCreated)
            *wasCreated = false;
        if(!rt)
            return Directory();
            
        static thread_local CacheQuery    i("INSERT OR FAIL INTO Directories (path,root,folder,parent) VALUES (?,?,1,0)");
        static thread_local CacheQuery    s("SELECT id FROM Directories WHERE path=?");
        
        auto s_lk   = s.af();
        auto i_lk   = i.af();
        i.bind(1, rt->path);
        i.bind(2, rt->id);
        
        if(is_good(i.step())){
            if(wasCreated)
                *wasCreated  = true;
            return Directory(i.last_id());
        } else {
            s.bind(1, rt->path);
            if(s.step() == SQResult::Row)
                return Directory(s.v_uint64(1));
            cdbError << "Unable to get directory ID";
            return Directory();
        }
    }

    Directory           directory(const Root*rt)
    {
        if(!rt)
            return Directory();
        return directory(rt->path);
    }
    
    std::vector<Directory>   directories(const Root*rt, Sorted sorted)
    {
        if(!rt)
            return std::vector<Directory>();
            
        static thread_local CacheQuery    qs("SELECT id FROM Directories WHERE parent=0 AND root=?");
        static thread_local CacheQuery    qu("SELECT id FROM Directories WHERE parent=0 AND root=? ORDER BY NAME");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Directory>(rt->id);
    }

    size_t              directories_count(const Root* rt)
    {
        if(!rt)
            return 0;
        static thread_local CacheQuery    s("SELECT COUNT(1) FROM Directories WHERE parent=0 AND root=?");
        return s.size(rt->id);
    }
    
    bool                exists(const Root*rt, std::string_view z)
    {
        if(!rt)
            return false;
        return std::filesystem::exists(rt -> resolve(z));
    }
    

    Fragment            fragment(const Root*rt, std::string_view z)
    {
        if(!rt)
            return Fragment{};
        return fragment(rt->resolve(z));
    }

    
    std::vector<Fragment>    fragments(const Root*rt, Sorted sorted)
    {
        return child_fragments(directory(rt), sorted);
    }

    std::string             key(const Root*rt)
    {
        if(!rt)
            return std::string();
        return rt->key;
    }
    
    std::filesystem::path   path(const Root*rt, std::string_view z, bool fMakePath)
    {
        std::filesystem::path   p =  rt -> resolve(z);
        if(fMakePath && !p.empty())
            make_parent_path(p);
        return p;
    }
    
    std::filesystem::path   path(const Root*rt, Document doc, bool fMakePath)
    {
        return path(rt, key(doc), fMakePath);
    }
}
