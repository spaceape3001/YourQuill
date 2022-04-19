////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////


namespace yq {
    namespace cdb {
        Vector<Directory>   all_directories(unsigned opts)
        {
            if(opts & BEST_SORT){
                if(opts & HIDDEN){
                    static thread_local SQ    s("SELECT id FROM Directories ORDER BY path");
                    return s.vec<Directory>();
                } else {
                    static thread_local SQ    s("SELECT id FROM Directories WHERE hidden=0 ORDER BY path");
                    return s.vec<Directory>();
                }
            } else {
                if(opts & HIDDEN){
                    static thread_local SQ    s("SELECT id FROM Directories");
                    return s.vec<Directory>();
                } else {
                    static thread_local SQ    s("SELECT id FROM Directories WHERE hidden=0");
                    return s.vec<Directory>();
                }
            }
        }
        
        Vector<Directory>   all_directories(Sorted sorted)
        {
            return all_directories(HIDDEN | (sorted ? BEST_SORT : 0));
        }

        size_t              all_directories_count()
        {
            static thread_local SQ    s("SELECT COUNT(1) FROM Directories");
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
            static thread_local SQ s("SELECT id FROM Directories WHERE parent=? AND name=?");
            return s.as<Directory>(d.id, k);
        }

        Fragment            child_fragment(Directory d, std::string_view k)
        {
            static thread_local SQ s("SELECT id FROM Fragments WHERE dir=? AND name=? LIMIT 1");
            return s.as<Fragment>(d.id, k);
        }

    #if 0
        std::string                 child_key(Directory);       //<! Key inside the directory (with extensions)
    #endif

        namespace {
            Vector<Directory>   child_directories_sorted(Directory d)
            {
                static thread_local SQ    s("SELECT id FROM Directories WHERE parent=? ORDER BY name");
                return s.vec<Directory>(d.id);
            }

            Vector<Directory>   child_directories_unsorted(Directory d)
            {
                static thread_local SQ    s("SELECT id FROM Directories WHERE parent=?");
                return s.vec<Directory>(d.id);
            }
        }
        
        Vector<Directory>   child_directories(Directory d, Sorted sorted)
        {
            return sorted ? child_directories_sorted(d) : child_directories_unsorted(d);
        }
        
        size_t              child_directories_count(Directory d)
        {
            static thread_local SQ    s("SELECT COUNT(1) FROM Directories WHERE parent=?");
            return s.size(d.id);
        }
        
        Set<Directory>             child_directories_set(Directory d)
        {
            static thread_local SQ    s("SELECT id FROM Directories WHERE parent=?");
            return s.set<Directory>(d.id);
        }

        namespace {
            Vector<DirString>   child_directories_with_names_sorted(Directory dir)
            {
                Vector<DirString>   ret;
                static thread_local SQ    s("SELECT id,name FROM Directories WHERE parent=?");
                s.bind(1,dir.id);
                while(s.step() == SqlQuery::Row){
                    ret.push_back(DirString(Directory(s.v_uint64(1)), s.v_text(1)));
                }
                s.reset();
                return ret;
            }
            
            Vector<DirString>   child_directories_with_names_unsorted(Directory dir)
            {
                Vector<DirString>   ret;
                static thread_local SQ    s("SELECT id,name FROM Directories WHERE parent=? ORDER BY path");
                s.bind(1,dir.id);
                while(s.step() == SqlQuery::Row){
                    ret.push_back(DirString(Directory(s.v_uint64(1)), s.v_text(1)));
                }
                s.reset();
                return ret;
            }
        }

        Vector<DirString>   child_directories_with_names(Directory dir, Sorted sorted)
        {
            return sorted ? child_directories_with_names_sorted(dir) : child_directories_with_names_unsorted(dir);
        }

        namespace {
            Vector<Fragment>    child_fragments_sorted(Directory d)
            {
                static thread_local SQ    s("SELECT id FROM Fragments WHERE dir=? ORDER BY path");
                return s.vec<Fragment>(d.id);
            }

            Vector<Fragment>    child_fragments_unsorted(Directory d)
            {
                static thread_local SQ    s("SELECT id FROM Fragments WHERE dir=?");
                return s.vec<Fragment>(d.id);
            }
        }
        
        Vector<Fragment>    child_fragments(Directory d, Sorted sorted)
        {
            return sorted ? child_fragments_sorted(d) : child_fragments_unsorted(d);
        }

        size_t              child_fragments_count(Directory d)
        {
            static thread_local SQ        s("SELECT COUNT(1) FROM Fragments WHERE dir=?");
            return s.size(d.id);
        }

        Set<Fragment>    child_fragments_set(Directory d)
        {
            static thread_local SQ    s("SELECT id FROM Fragments WHERE dir=?");
            return s.set<Fragment>(d.id);
        }

        Vector<DirOrFrag>   children(Directory d, Sorted sorted)
        {
            Vector<DirOrFrag>   ret;
            for(Directory d2 : child_directories(d, sorted))
                ret.push_back(d2);
            for(Fragment f : child_fragments(d, sorted))
                ret.push_back(f);
            return ret;
        }
        
        Vector<DirOrFragStr>    children_and_names(Directory);  // TODO

        std::pair<Directory, Folder>    db_directory(Directory dirParent, std::string_view k, bool *wasCreated)
        {
            if(wasCreated)
                *wasCreated = false;
            if(k.empty() || !dirParent)
                return {};
            const Root* rt  = root(dirParent);
            if(!rt)
                return {};

            Folder f           = db_folder(folder(dirParent), k);
            if(!f)
                return {};
                
            std::string p       = path(dirParent);
            p += '/';
            p += k;
            
                
            static thread_local SQ    i("INSERT OR FAIL INTO Directories (path,root,folder,parent,name) VALUES (?,?,?,?,?)");
            static thread_local SQ    s("SELECT id FROM Directories WHERE path=?");
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
                if(s.step() == SqlQuery::Row)
                    return { Directory(s.v_uint64(1)), f };
                cdbError << "Unable to get directory ID";
                return {};
            }
        }
        
        //Fragment                db_fragment(Document, const Root*); // TODO

        Directory           directory(uint64_t i)
        {
            return exists_directory(i) ? Directory{i} : Directory();
        }

        Directory           directory(const std::filesystem::path&p)
        {
            if(p.empty())
                return Directory();
            static thread_local SQ    s("SELECT id FROM Directories WHERE path=? LIMIT 1");
            return s.as<Directory>(p);
        }
        
        //Vector<Document>    documents(Directory);

        void    erase(Directory d)
        {
            if(!d)
                return ;
            
            Folder          f   = folder(d);
            if(f.id && f.id <= kMaxFixedFolder)
                return ;
            
            {
                static thread_local SQ dq("DELETE FROM Directories WHERE id=?");
                dq.bind(1, d.id);
                dq.step();
                dq.reset();
            }
            
            if(!f)
                return ;
            
            size_t dc = directories_count(f);
            if(!dc){
                static thread_local SQ dq("DELETE FROM Folders WHERE id=?");
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
            static thread_local SQ s("SELECT 1 FROM Directories WHERE id=? LIMIT 1");
            return s.present(i);
        }

        Folder              folder(Directory d)
        {
            static thread_local SQ    s("SELECT folder FROM Directories WHERE id=?");
            return s.as<Folder>(d.id);
        }

        Vector<Folder>      folder_path(Directory d)
        {
            return folder_path(folder(d));
        }


        bool                hidden(Directory d)
        {
            static thread_local SQ    s("SELECT hidden FROM Directories WHERE id=?");
            return s.boolean(d.id);
        }
        
        Directory::Info     info(Directory d)
        {
            Directory::Info        ret;
            static thread_local SQ    s("SELECT folder, name, parent, path, removed, root, hidden FROM Directories WHERE id=?");
            s.bind(1, d.id);
            if(s.step() == SqlQuery::Row){
                ret.folder  = Folder(s.v_uint64(1));
                ret.name    = s.v_text(2);
                ret.parent  = Directory(s.v_uint64(3));
                ret.path    = s.v_text(4);
                ret.removed = s.v_bool(5);
                ret.root    = wksp::root(s.v_uint64(6));
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
            static thread_local SQ    s("SELECT name FROM Directories WHERE id=?");
            return s.str(d.id);
        }
        
        Directory           parent(Directory d)
        {
            static thread_local SQ    s("SELECT parent FROM Directories WHERE id=?");
            return s.as<Directory>(d.id);
        }

        std::filesystem::path   path(Directory d)
        {
            static thread_local SQ    s("SELECT path FROM Directories WHERE id=?");
            return s.path(d.id);
        }
       
        bool                removed(Directory d)
        {
            static thread_local SQ    s("SELECT removed FROM Directories WHERE id=?");
            return s.boolean(d.id);
        }
        
        const Root*         root(Directory d)
        {
            static thread_local SQ    s("SELECT root FROM Directories WHERE id=?");
            auto s_lk   = s.af();
            s.bind(1, d.id);
            if(s.step() == SqlQuery::Row)
                return wksp::root(s.v_uint64(1));
            return nullptr;
        }
        
        std::string             skey(Directory d)
        {
            static thread_local SQ    s("SELECT name FROM Documents WHERE id=?");
            return s.str(d.id);
        }

        std::string             skeyb(Directory d)
        {
            std::string     bk  = skey(d);
            return copy(base_key(bk));
        }
    }
}
