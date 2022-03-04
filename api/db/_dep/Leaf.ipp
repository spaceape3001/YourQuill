////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace yq {
    namespace cdb {


        namespace {
            Vector<Atom>    all_leaf_atoms_sorted()
            {
                static thread_local SQ s("SELECT id FROM Atoms WHERE isLeaf=1 ORDER BY k");
                return s.vec<Atom>();
            }
            
            Vector<Atom>    all_leaf_atoms_unsorted()
            {
                static thread_local SQ s("SELECT id FROM Atoms WHERE leaf!=0");
                return s.vec<Atom>();
            }
        }
        
        
        Vector<Atom>        all_leaf_atoms(Sorted sorted)
        {
            return sorted ? all_leaf_atoms_sorted() : all_leaf_atoms_unsorted();
        }
        
        size_t              all_leaf_atoms_count()
        {
            static thread_local SQ s("SELECT COUNT(1) FROM Atoms WHERE leaf!=0");
            return s.size();
        }
        
        namespace {
            Vector<Leaf>    all_leafs_sorted()
            {
                static thread_local SQ s("SELECT id FROM Leafs ORDER BY k");
                return s.vec<Leaf>();
            }

            Vector<Leaf>    all_leafs_unsorted()
            {
                static thread_local SQ s("SELECT id FROM Leafs");
                return s.vec<Leaf>();
            }
        }
        
        Vector<Leaf>        all_leafs(Sorted sorted)
        {
            return sorted ? all_leafs_sorted() : all_leafs_unsorted();
        }
        
        size_t              all_leafs_count()
        {
            static thread_local SQ s("SELECT COUNT(1) FROM Leafs");
            return s.size();
        }
        

        Atom                atom(Leaf l)
        {
            static thread_local SQ s("SELECT atom FROM Leafs WHERE id=? LIMIT 1");
            return s.as<Atom>(l.id);
        }
        
        
        Leaf                db_leaf(Document doc, bool*wasCreated)
        {
            if(wasCreated)
                *wasCreated = false;
            if(!doc.id){
                yError() << "Cannot create to a null document!";
                return Leaf();
            }
            
            String     k   = strip_extension(key(doc));
            if(k.empty()){
                yError() << "Cannot create to blank key!";
                return Leaf();
            }
            
            static thread_local SQ i("INSERT INTO Leafs (id,k) VALUES (?,?)");
            auto i_af   = i.af();
            i.bind(0,doc.id);
            i.bind(1, k);
            if(i.exec(false)){
                if(wasCreated)
                    *wasCreated = true;
                return Leaf(doc.id);
            } else if(exists_leaf(doc.id)){
                return Leaf(doc.id);
            } else {
                yError() << "Unable to create or find the leaf: " << k;
                return Leaf();
            }
        }

        Folder              detail_folder(Leaf l)
        {
            Folder  f   = folder(Document{l.id});
            String sk  = skeyb(Document{l.id});
            return db_folder(f, sk+".d");
        }
        
        Document            document(Leaf l)
        {
            return exists(l) ?  Document{l.id} : Document{};
        }
        
        bool                exists(Leaf l)
        {
            return exists_leaf(l.id);
        }
        

        bool                exists_leaf(uint64_t i)
        {
            static thread_local SQ s("SELECT 1 FROM Leafs WHERE id=? LIMIT 1");
            return s.present(i);
        }

        Folder              folder(Leaf l)
        {
            return folder(document(l));
        }
        
        
        Image               icon(Leaf l)
        {
            static thread_local SQ    s("SELECT icon FROM Leafs WHERE id=? LIMIT 1");
            return s.as<Image>(l.id);
        }
        

        Leaf::Info          info(Leaf l, bool autoKey)
        {
            Leaf::Info    ret;
            static thread_local SQ s("SELECT k, title FROM Leafs WHERE id=?");
            auto s_af = s.af();
            s.bind(0, l.id);
            if(s.exec() && s.next()){
                ret.doc     = Document(l.id);
                ret.key     = s.valueString(0);
                ret.title   = s.valueString(1);
                if(autoKey && ret.title.empty())
                    ret.title   = ret.key;
            }
            return ret;
        }

        //bool                    is_leaf(Atom);
        
        
        String             key(Leaf l)
        {
            static thread_local SQ s("SELECT k FROM Leafs WHERE id=?");
            return s.str(l.id);
        }

        String             label(Leaf l)
        {
            String     n = title(l);
            return n.empty() ? key(l) : n;
        }
        
        
        Leaf                leaf(uint64_t i)
        {
            return exists_leaf(i) ? Leaf{i} : Leaf();
        }

        Leaf                leaf(const String& k)
        {
            static thread_local SQ s("SELECT id FROM Leafs WHERE k=?");
            return s.as<Leaf>(k);
        }

        Leaf                leaf(Atom a)
        {
            static thread_local SQ    s("SELECT leaf FROM Atoms WHERE id=?");
            return s.as<Leaf>(a.id);
        }
        
        
        Leaf                leaf(Document d)
        {
            return exists_leaf(d.id) ? Leaf{d.id} : Leaf{};
        }
        
        Leaf                leaf(Folder f)
        {
            return leaf(key(f));
        }

        Leaf                    leaf_by_title(const String&k)
        {
            static thread_local SQ    s("SELECT id FROM Leafs WHERE title like ? LIMIT 1");
            return s.as<Leaf>(k);
        }
        
        LeafFile::Shared         leaf_doc(Fragment f, unsigned int options)
        {
            if(!f)
                return LeafFile::Shared();

            auto    ch   = frag_bytes(f);
            if(ch.empty())
                return (options & AllowEmpty) ? std::make_shared<LeafFile>() : LeafFile::Shared();
            
            LeafFile::Shared     td  = std::make_shared<LeafFile>();
            std::filesystem::path   fp  = path(f);
            if(!td->load(std::move(ch), fp)){
                yError() << "Unable to read " << fp;
                return LeafFile::Shared();
            }
            td -> set_file(fp);
            return td;
        }

        
        Leaf::Merge::Shared         merged(Leaf l, unsigned int opts)
        {
            Leaf::Merge::Shared     ret = std::make_shared<Leaf::Merge>();
            bool    fFragUpdate     = static_cast<bool>(opts & IsUpdate);
            //bool    fAttributes     = !static_cast<bool>(opts & IgnoreAttributes);
            bool    fContext        = !static_cast<bool>(opts & IgnoreContext);
                
            for(auto& i : reads(l)){
            
                if(fFragUpdate)
                    update(i.first);
            
                const Root* rt  = root(i.first);
                    
                if(fContext && !i.second->body.empty())
                    ret->context[rt] = i.second->context();
                ret->fusion(*i.second);
            }
            return ret;
        }
        

        
        NKI                 nki(Leaf l, bool autoKey)
        {
            static thread_local SQ    s("SELECT title,icon,k FROM Leafs WHERE id=?");
            auto s_af = s.af();
            s.bind(0, l.id);
            if(s.exec() && s.next()){
                NKI  ret;
                ret.name    = s.valueString(0);
                ret.icon    = Image(s.valueU64(1)) ;
                ret.key     = s.valueString(2);
                if(autoKey && ret.name.empty())
                    ret.name    = ret.key;
                return ret;
            }
            return NKI{};
        }

        LeafFile::Shared         read(Leaf l, const Root* rt)
        {
            return leaf_doc(fragment(document(l), rt));
        }

        Vector<Leaf::FragDoc>     reads(Leaf l)
        {
            Vector<Leaf::FragDoc>  ret;
            for(Fragment f : fragments(document(l), DataRole::DB)){
                LeafFile::Shared p   = leaf_doc(f);
                if(p)
                    ret << Leaf::FragDoc(f,p);
            }
            return ret;
        }

        Vector<Leaf::FragDoc>   reads(Leaf l, class Root*rt)
        {
            Vector<Leaf::FragDoc>  ret;
            for(Fragment f : fragments(document(l), rt)){
                LeafFile::Shared p   = leaf_doc(f);
                if(p)
                    ret << Leaf::FragDoc(f,p);
            }
            return ret;
        }

        String             title(Leaf l)
        {
            static thread_local SQ s("SELECT title FROM Leafs WHERE id=?");
            return s.str(l.id);
        }

        LeafFile::Shared         write(Leaf l, const Root* rt)
        {
            Document    d   = document(l);
            if(!d)
                return LeafFile::Shared();
            Fragment    f   = fragment(d);
            if(f)
                return leaf_doc(f, true);
            Folder      fo  = folder(d);
            if((fo != cdb::top_folder()) && !exists(fo, rt))
                rt -> make_path(key(fo));
            LeafFile::Shared ptr  = std::make_shared<LeafFile>();
            ptr -> set_file( rt -> resolve(key(d)));
            return ptr;
        }

    }
}
