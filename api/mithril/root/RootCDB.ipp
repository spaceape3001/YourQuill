////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RootCDB.hpp"
#include <0/io/DirUtils.hpp>
#include <mithril/directory/DirectoryCDB.hpp>
#include <mithril/document/DocumentCDB.hpp>
#include <mithril/fragment/FragmentCDB.hpp>
#include <mithril/root/Root.hpp>
#include <mithril/root/RootDir.hpp>
#include <mithril/wksp/CacheQuery.hpp>
#include <mithril/wksp/Workspace.hpp>
#include <mithril/wksp/CacheLogging.hpp>

namespace yq::mithril::cdb {
    DirectoryVector   all_directories(const RootDir*rt, Sorted sorted)
    {
        if(!rt)
            return DirectoryVector();

        static thread_local CacheQuery    qs("SELECT id FROM " TBL_DIRECTORIES " WHERE root=? ORDER BY path");
        static thread_local CacheQuery    qu("SELECT id FROM " TBL_DIRECTORIES " WHERE root=?");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Directory>(rt->id);
    }
    
    DirectoryVector     all_directories(Root rt, Sorted sorted)
    {
        return all_directories(root_dir(rt), sorted);
    }

    size_t              all_directories_count(const RootDir*rt)
    {
        if(!rt)
            return 0;

        static thread_local CacheQuery    s("SELECT COUNT(1) FROM " TBL_DIRECTORIES " WHERE root=?");
        return s.size(rt->id);
    }
    
    FragmentVector   all_fragments(const RootDir*rt, Sorted sorted)
    {
        if(!rt)
            return std::vector<Fragment>();
        static thread_local CacheQuery qs("SELECT id FROM " TBL_FRAGMENTS " WHERE root=? ORDER BY path");
        static thread_local CacheQuery qu("SELECT id FROM " TBL_FRAGMENTS " WHERE root=?");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Fragment>(rt->id);
    }
    
    size_t              all_fragments_count(const RootDir*rt)
    {
        if(!rt)
            return 0;
        static thread_local CacheQuery s("SELECT COUNT(1) FROM " TBL_FRAGMENTS " WHERE root=?");
        return s.size(rt->id);
    }

    std::vector<Root>           all_roots(Sorted sorted)
    {
        size_t  n   = all_roots_count();
        std::vector<Root>   ret;
        ret.reserve(n);
        for(size_t i=1;i<=n;++i)
            ret.push_back(Root(i));
        return ret;
    }

    size_t                      all_roots_count()
    {
        return wksp::root_dirs().size();
    }
    
    std::vector<DirOrFrag>   children(const RootDir* rt, Sorted sorted)
    {
        return children(directory(rt), sorted);
    }
    
    std::vector<uint8_t>         data(Fragment);     // TODO
    
    Directory           db_root(const RootDir*rt, bool *wasCreated)
    {
        if(wasCreated)
            *wasCreated = false;
        if(!rt)
            return Directory();
            
        static thread_local CacheQuery    i("INSERT OR FAIL INTO " TBL_DIRECTORIES " (path,root,folder,parent) VALUES (?,?,1,0)");
        static thread_local CacheQuery    s("SELECT id FROM " TBL_DIRECTORIES " WHERE path=?");
        
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

    Directory                  directory(Root r)
    {
        return directory(root_dir(r));
    }


    Directory           directory(const RootDir*rt)
    {
        if(!rt)
            return Directory();
        return directory(rt->path);
    }
    
    DirectoryVector   directories(const RootDir*rt, Sorted sorted)
    {
        if(!rt)
            return DirectoryVector();
            
        static thread_local CacheQuery    qs("SELECT id FROM " TBL_DIRECTORIES " WHERE parent=0 AND root=?");
        static thread_local CacheQuery    qu("SELECT id FROM " TBL_DIRECTORIES " WHERE parent=0 AND root=? ORDER BY NAME");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Directory>(rt->id);
    }

    size_t              directories_count(const RootDir* rt)
    {
        if(!rt)
            return 0;
        static thread_local CacheQuery    s("SELECT COUNT(1) FROM " TBL_DIRECTORIES " WHERE parent=0 AND root=?");
        return s.size(rt->id);
    }
    
    bool                exists(const RootDir*rt, std::string_view z)
    {
        if(!rt)
            return false;
        return std::filesystem::exists(rt -> resolve(z));
    }
    

    Fragment            fragment(const RootDir*rt, std::string_view z)
    {
        if(!rt)
            return Fragment{};
        return fragment(rt->resolve(z));
    }

    
    FragmentVector   fragments(const RootDir*rt, Sorted sorted)
    {
        return child_fragments(directory(rt), sorted);
    }

    Image                       icon(Root)
    {
        return Image();     // TODO
    }

    bool                    is_template(Root rt)
    {
        const RootDir* r    = root_dir(rt);
        return r ? r -> is_template : false;
    }
    
    std::string             key(const RootDir*rt)
    {
        if(!rt)
            return std::string();
        return rt->key;
    }
    
    std::string                key(Root rt)
    {
        return key(root_dir(rt));
    }

    std::string                 name(Root rt)
    {
        const RootDir* r    = root_dir(rt);
        return r ? r -> name : std::string();
    }

    std::filesystem::path   path(Root rt)
    {
        const RootDir* r =  root_dir(rt);
        return r ? r -> path : std::filesystem::path();
    }
    
    std::filesystem::path   path(const RootDir*rt, std::string_view z, bool fMakePath)
    {
        if(!rt)
            return std::filesystem::path();
        std::filesystem::path   p =  rt -> resolve(z);
        if(fMakePath && !p.empty())
            make_parent_path(p);
        return p;
    }
    
    std::filesystem::path   path(const RootDir*rt, Document doc, bool fMakePath)
    {
        return path(rt, key(doc), fMakePath);
    }
}
