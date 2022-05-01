////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace yq {
    namespace cdb {
       namespace {
            std::vector<Folder>  all_folders_sorted()
            {
                static thread_local SQ    s("SELECT id FROM Folders ORDER BY k");
                return s.vec<Folder>();
            }

            std::vector<Folder>  all_folders_unsorted()
            {
                static thread_local SQ    s("SELECT id FROM Folders");
                return s.vec<Folder>();
            }
        }

        std::vector<Folder>      all_folders(Sorted sorted)
        {
            return sorted ? all_folders_sorted() : all_folders_unsorted();
        }
        
        size_t              all_folders_count()
        {
            static thread_local SQ s("SELECT COUNT(1) FROM Folders");
            return s.size();
        }

        std::string             brief(Folder f)
        {
            static thread_local SQ    s("SELECT brief FROM Folders WHERE id=?");
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
            static thread_local SQ s("SELECT id FROM Documents WHERE folder=? AND sk=?");
            return s.as<Document>(f.id, k);
        }
        
        std::vector<Document>    child_documents(Folder f, unsigned opts)
        {
            if(opts & BEST_SORT){
                if(opts & HIDDEN){
                    static thread_local SQ    s("SELECT id FROM Documents WHERE folder=? ORDER BY sk");
                    return s.vec<Document>(f.id);
                } else {
                    static thread_local SQ    s("SELECT id FROM Documents WHERE folder=? AND hidden=0 ORDER BY sk");
                    return s.vec<Document>(f.id);
                }
            } else {
                if(opts & HIDDEN){
                    static thread_local SQ    s("SELECT id FROM Documents WHERE folder=?");
                    return s.vec<Document>(f.id);
                } else {
                    static thread_local SQ    s("SELECT id FROM Documents WHERE folder=? AND hidden=0");
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
                static thread_local SQ    s("SELECT COUNT(1) FROM Documents WHERE folder=?");
                return s.size(f.id);
            } else {
                static thread_local SQ    s("SELECT COUNT(1) FROM Documents WHERE folder=? AND hidden=0");
                return s.size(f.id);
            }
        }
        
        namespace {
            std::vector<Document>    child_documents_by_suffix_sorted(Folder f, std::string_view sfx)
            {
                static thread_local SQ    s("SELECT id FROM Documents WHERE folder=? AND suffix=? ORDER BY k");
                return s.vec<Document>(f.id, sfx);
            }

            std::vector<Document>    child_documents_by_suffix_unsorted(Folder f, std::string_view sfx)
            {
                static thread_local SQ    s("SELECT id FROM Documents WHERE folder=? AND suffix=?");
                return s.vec<Document>(f.id, sfx);
            }
        }
        
        std::vector<Document>    child_documents_by_suffix(Folder f, std::string_view sfx, Sorted sorted)
        {
            return sorted ? child_documents_by_suffix_sorted(f, sfx) : child_documents_by_suffix_unsorted(f,sfx);
        }
        
        
        namespace {
            std::vector<Document>    child_documents_by_suffix_excluding_sorted(Folder f, std::string_view sfx)
            {
                static thread_local SQ    s("SELECT id FROM Documents WHERE folder!=? AND suffix=? ORDER BY k");
                return s.vec<Document>(f.id, sfx);
            }
            
            std::vector<Document>    child_documents_by_suffix_excluding_unsorted(Folder f, std::string_view sfx)
            {
                static thread_local SQ    s("SELECT id FROM Documents WHERE folder!=? AND suffix=?");
                return s.vec<Document>(f.id, sfx);
            }
        }
        
        std::vector<Document>    child_documents_by_suffix_excluding(Folder f, std::string_view sfx, Sorted sorted)
        {
            return sorted ? child_documents_by_suffix_excluding_sorted(f,sfx) : child_documents_by_suffix_excluding_unsorted(f,sfx);
        }

        Folder              child_folder(Folder f, std::string_view ck)
        {
            static thread_local SQ    s("SELECT id FROM Folders WHERE parent=? AND ck=? LIMIT 1");
            return s.as<Folder>(f.id, ck);
        }

        std::vector<Folder>      child_folders(Folder f, unsigned opts)
        {
            if(opts & BEST_SORT){
                if(opts & HIDDEN){
                    static thread_local SQ    s("SELECT id FROM Folders WHERE parent=? ORDER BY sk");
                    return s.vec<Folder>(f.id);
                } else {
                    static thread_local SQ    s("SELECT id FROM Folders WHERE parent=? AND hidden=0 ORDER BY sk");
                    return s.vec<Folder>(f.id);
                }
            } else {
                if(opts & HIDDEN){
                    static thread_local SQ    s("SELECT id FROM Folders WHERE parent=?");
                    return s.vec<Folder>(f.id);
                } else {
                    static thread_local SQ    s("SELECT id FROM Folders WHERE parent=? AND hidden=0");
                    return s.vec<Folder>(f.id);
                }
            }
        }

        std::vector<Folder>      child_folders(Folder f, Sorted sorted)
        {
            return child_folders(f, HIDDEN | (sorted ? BEST_SORT : 0));
        }
        
        
        size_t              child_folders_count(Folder f, unsigned opts)
        {
            if(opts & HIDDEN){
                static thread_local SQ    s("SELECT COUNT(1) FROM Folders WHERE parent=?");
                return s.size(f.id);
            } else {
                static thread_local SQ    s("SELECT COUNT(1) FROM Folders WHERE parent=? AND hidden=0");
                return s.size(f.id);
            }
        }


        namespace {
            std::vector<Fragment>    child_fragments_sorted(Folder f)
            {
                static thread_local SQ    s("SELECT id FROM Fragments WHERE folder=? ORDER BY path");
                return s.vec<Fragment>(f.id);
            }

            std::vector<Fragment>    child_fragments_unsorted(Folder f)
            {
                static thread_local SQ    s("SELECT id FROM Fragments WHERE folder=?");
                return s.vec<Fragment>(f.id);
            }
        }
        
        std::vector<Fragment>    child_fragments(Folder f, Sorted sorted)
        {
            return sorted ? child_fragments_sorted(f) : child_fragments_unsorted(f);
        }
        
        size_t              child_fragments_count(Folder f)
        {
            static thread_local SQ        s("SELECT COUNT(1) FROM Fragments WHERE folder=?");
            return s.size(f.id);
        }

    #if 0
        std::string                 child_key(Folder);
    #endif

        std::vector<DocOrFold>   children(Folder, Sorted sorted);           // TODO
        std::vector<DocOrFoldStr>    children_and_names(Folder);     // TODO
        
        
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
                
            static thread_local SQ    i("INSERT OR FAIL INTO Folders (k,sk,name,parent,hidden) VALUES (?,?,?,?,?)");
            static thread_local SQ    s("SELECT id FROM Folders WHERE k=?");

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
                if(s.step() == SqlQuery::Row)
                    return Folder{s.v_uint64(1)};
                cdbError << "Unable to get folder ID";
                return Folder{};
            }    
        }
        
        namespace {
            std::vector<Directory>   directories_sorted(Folder f)
            {
                static thread_local SQ    s("SELECT id FROM Directories WHERE folder=? ORDER BY name");
                return s.vec<Directory>(f.id);
            }
            
            std::vector<Directory>   directories_unsorted(Folder f)
            {
                static thread_local SQ    s("SELECT id FROM Directories WHERE folder=?");
                return s.vec<Directory>(f.id);
            }
        }
        
        std::vector<Directory>   directories(Folder f, Sorted sorted)
        {
            return sorted ? directories_sorted(f) : directories_unsorted(f);
        }
        
        size_t              directories_count(Folder f)
        {
            static thread_local SQ    s("SELECT COUNT(1) FROM Directories WHERE folder=?");
            return s.size(f.id);
        }
        
        bool                exists(Folder f)
        {
            return exists_folder(f.id);
        }

        bool                exists(Folder fo, const Root*rt)
        {
            if(!rt)
                return false;
                
            static thread_local SQ s("SELECT 1 FROM Directories WHERE folder=? AND root=? LIMIT 1");
            return s.present(fo.id, rt->id);
        }

        bool                exists_folder(uint64_t i)
        {
            static thread_local SQ s("SELECT 1 FROM Folders WHERE id=? LIMIT 1");
            return s.present(i);
        }

        Directory           first_directory(Folder fo)
        {
            if(!fo)
                return Directory();
            static thread_local SQ  s("SELECT id FROM Directories WHERE folder=? LIMIT 1");
            return s.as<Directory>(fo.id);
        }
        
        Directory           first_directory(Folder fo, const Root*r)
        {
            if(!r)
                return first_directory(fo);
            if(!fo)
                return Directory();
            
            static thread_local SQ s("SELECT id FROM Directories WHERE folder=? AND root=? LIMIT 1");
            auto af = s.af();
            s.bind(1, fo.id);
            s.bind(2, r->id);
            if(s.step() == SqlQuery::Row)
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

        Folder              folder(uint64_t i)
        {
            return exists_folder(i) ? Folder{i} : Folder{};
        }
        
        Folder              folder(std::string_view k)
        {
            static thread_local SQ    s("SELECT id FROM Folders WHERE k=? LIMIT 1");
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
            static thread_local SQ    s("SELECT hidden FROM Folders WHERE id=?");
            return s.boolean(f.id);
        }
        
        Image               icon(Folder f)
        {
            static thread_local SQ    s("SELECT icon FROM Folders WHERE id=? LIMIT 1");
            return s.as<Image>(f.id);
        }
        
        Folder::Info        info(Folder f)
        {
            Folder::Info        ret;
            static thread_local SQ    s("SELECT k, sk, parent, name, brief, hidden, removed, icon FROM Folders WHERE id=?");
            s.bind(1,f.id);
            if(s.step() == SqlQuery::Row){
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
            static thread_local SQ    s("SELECT k FROM Folders WHERE id=?");
            return s.str(f.id);
        }

        std::string             label(Folder f)
        {
            return key(f);
        }
        
        void    make_directory(Folder fo, const Root* rt)
        {
            if(!rt)
                return ;
            if(fo == top_folder())
                return ;
                
            make_path(rt->resolve(key(fo)));
        }

        std::string             name(Folder f)
        {
            static thread_local SQ    s("SELECT name FROM Folders WHERE id=?");
            return s.str(f.id);
        }

        NKI                 nki(Folder f, bool autoKey)
        {
            static thread_local SQ    s("SELECT name,icon,k FROM Folders WHERE id=?");
            auto s_af = s.af();
            s.bind(1, f.id);
            if(s.step() == SqlQuery::Row){
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
            static thread_local SQ    s("SELECT parent FROM Folders WHERE id=?");
            return s.as<Folder>(f.id);
        }
        
        bool                removed(Folder f)
        {
            static thread_local SQ    s("SELECT removed FROM Folders WHERE id=?");
            return s.boolean(f.id);
        }
        
        std::vector<const Root*> roots(Folder f)
        {
            std::vector<const Root*> ret;
            static thread_local SQ    s("SELECT DISTINCT root FROM Directories WHERE folder=?");
            auto s_af       = s.af();
            s.bind(1, f.id);
            while(s.step() == SqlQuery::Row){
                const Root*r    = wksp::root(s.v_uint64(1));
                if(r)
                    ret.push_back(r);
            }
            return ret;
        }
        
        size_t              roots_count(Folder f)
        {
            static thread_local SQ    s("SELECT COUNT(DISTINCT root) FROM Directories WHERE folder=?");
            return s.size(f.id);
        }

        std::string             skey(Folder f)
        {
            static thread_local SQ    s("SELECT sk FROM Folders WHERE id=?");
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
}
