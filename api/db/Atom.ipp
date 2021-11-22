////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace cdb {
    namespace {
        String     full_key(Document d, const String& ck)
        {
            String     dk = strip_extension(key(d));
            if(dk.empty())    
                return String();

            if(!ck.empty())
                dk      += "#" + ck;
            return dk;
        }
    }


    String             abbreviation(Atom a)
    {
        static thread_local SQ s("SELECT abbr FROM Atoms WHERE id=?");
        return s.str(a.id);
    }

    namespace {
        AtomVec        all_atoms_sorted()
        {
            static thread_local SQ s("SELECT id FROM Atoms ORDER BY k");
            return s.vec<Atom>();
        }

        AtomVec        all_atoms_unsorted()
        {
            static thread_local SQ s("SELECT id FROM Atoms");
            return s.vec<Atom>();
        }
    }
    

    AtomVec        all_atoms(Sorted sorted)
    {
        return sorted ? all_atoms_sorted() : all_atoms_unsorted();
    }
    
    namespace {
        AtomVec    all_atoms_sorted(Class c)
        {
                    // I think this SQL is right.....
            static thread_local SQ s("SELECT atom FROM AClasses INNER JOIN Classes ON AClasses.class=Classes.id WHERE class=? ORDER BY Classes.k");
            return s.vec<Atom>(c.id);
        }

        AtomVec    all_atoms_unsorted(Class c)
        {
            static thread_local SQ s("SELECT atom FROM AClasses WHERE class=?");
            return s.vec<Atom>(c.id);
        }
    }
    
    AtomVec        all_atoms(Class c,Sorted sorted)
    {
        return sorted ? all_atoms_sorted(c) : all_atoms_unsorted(c);
    }
    
    namespace {
        AtomVec    all_atoms_sorted(Tag t)
        {
            static thread_local SQ s("SELECT atom FROM ATags INNER JOIN Tags ON ATags.tag=Tags.id WHERE tag=? ORDER BY Tags.k");
            return s.vec<Atom>(t.id);
        }

        AtomVec    all_atoms_unsorted(Tag t)
        {
            static thread_local SQ s("SELECT atom FROM ATags WHERE tag=?");
            return s.vec<Atom>(t.id);
        }
    }
    
    AtomVec        all_atoms(Tag t,Sorted sorted)
    {
        return sorted ? all_atoms_sorted(t) : all_atoms_unsorted(t);
    }
    
    size_t              all_atoms_count()
    {
        static thread_local SQ s("SELECT COUNT(1) FROM Atoms");
        return s.size();
    }
    
    size_t              all_atoms_count(Class c)
    {
        static thread_local SQ s("SELECT COUNT(1) FROM AClasses WHERE class=?");
        return s.size(c.id);
    }
    
    size_t              all_atoms_count(Tag t)
    {
        static thread_local SQ s("SELECT COUNT(1) FROM ATags WHERE tag=?");
        return s.size(t.id);
    }
    
    
    Atom                atom(uint64_t i)
    {
        return exists_atom(i) ? Atom{i} : Atom();
    }
    
    Atom                atom(const String& k)
    {
        static thread_local SQ s("SELECT id FROM Atoms WHERE k=? LIMIT 1");
        return s.as<Atom>(k);
    }

    Atom                atom(Document doc, const String& ck)
    {
        return atom(full_key(doc, ck));
    }
    
    
    namespace {
        AtomVec        atoms_sorted(Atom a)
        {
            static thread_local SQ s("SELECT id FROM Atoms WHERE parent=? ORDER BY k");
            return s.vec<Atom>(a.id);
        }
        
        AtomVec        atoms_unsorted(Atom a)
        {
            static thread_local SQ s("SELECT id FROM Atoms WHERE parent=?");
            return s.vec<Atom>(a.id);
        }
        
    }

    AtomVec            atoms(Atom a, Sorted sorted)
    {
        return sorted ? atoms_sorted(a) : atoms_unsorted(a);
    }

    namespace {
        AtomVec    atoms_sorted(Document doc)
        {
            static thread_local SQ s("SELECT atom FROM ADocuments INNER JOIN Atoms ON ADocuments.atom=Atoms.id WHERE doc=? ORDER BY Atoms.k");
            return s.vec<Atom>(doc.id);
        }

        AtomVec    atoms_unsorted(Document doc)
        {
            static thread_local SQ s("SELECT atom FROM ADocuments WHERE doc=?");
            return s.vec<Atom>(doc.id);
        }
    }
    
    AtomVec        atoms(Document doc, Sorted sorted)
    {
        return sorted ? atoms_sorted(doc) : atoms_unsorted(doc);
    }
    
    namespace {
        AtomVec     atoms_by_name_sorted(const String& n)
        {
            static thread_local SQ s("SELECT id FROM Atoms WHERE name=? ORDER BY k");
            return s.vec<Atom>(n);
        }
        
        AtomVec     atoms_by_name_unsorted(const String& n)
        {
            static thread_local SQ s("SELECT id FROM Atoms WHERE name=?");
            return s.vec<Atom>(n);
        }
    }
    
    
    AtomVec             atoms_by_name(const String& n, Sorted sorted)
    {
        return sorted ? atoms_by_name_sorted(n) : atoms_by_name_unsorted(n);
        AtomVec     ret;
    }

    size_t              atoms_count(Document doc)
    {
        static thread_local SQ s("SELECT COUNT(1) FROM Atoms WHERE doc=?");
        return s.size(doc);
    }
    
    String             brief(Atom a)
    {
        static thread_local SQ s("SELECT brief FROM Atoms WHERE id=?");
        return s.str(a.id);
    }
    
    namespace {
        ClassVec    classes_sorted(Atom a)
        {
            static thread_local SQ s("SELECT class FROM AClasses INNER JOIN Classes ON AClasses.class=Classes.id WHERE atom=? ORDER BY Classes.k");
            return s.vec<Class>(a.id);
        }
        
        ClassVec    classes_unsorted(Atom a)
        {
            static thread_local SQ s("SELECT class FROM AClasses WHERE atom=?");
            return s.vec<Class>(a.id);
        }
    }
    
    
    ClassVec       classes(Atom a, Sorted sorted)
    {
        return sorted ? classes_sorted(a) : classes_unsorted(a);
    }
    
    namespace {
        ClassVec    classes_sorted(Atom a, Document d)
        {
            static thread_local SQ s("SELECT class FROM AClasses INNER JOIN Classes ON AClasses.class=Classes.id WHERE atom=? AND doc=? ORDER BY Classes.k");
            return s.vec<Class>(a.id, d.id);
        }

        ClassVec    classes_unsorted(Atom a, Document d)
        {
            static thread_local SQ s("SELECT class FROM AClasses WHERE atom=? AND doc=?");
            return s.vec<Class>(a.id, d.id);
        }
    }
    
    ClassVec       classes(Atom a, Document d, Sorted sorted)
    {
        return sorted ? classes_sorted(a,d) : classes_unsorted(a,d);
    }
    
    size_t              classes_count(Atom a)
    {
        static thread_local SQ s("SELECT COUNT(DISTINCT class) FROM AClasses WHERE atom=?");
        return s.size(a.id);
    }    
    
    size_t              classes_count(Atom a, Document d)
    {
        static thread_local SQ s("SELECT COUNT(1) FROM AClasses WHERE atom=? AND doc=?");
        return s.size(a.id, d.id);
    }    


    Atom                db_atom(Document d, bool* wasCreated)
    {
        return db_atom(d, String(), wasCreated);
    }
    
    Atom                db_atom(Document d, const String&ck, bool* wasCreated)
    {
        if(wasCreated)
            *wasCreated = false;


        String     dk = full_key(d, ck);
        if(dk.empty()){
            yError() << "Cannot create to an empty key!";
            return Atom();
        }

        static thread_local SQ    i("INSERT OR FAIL INTO Atoms (k,doc,sk) VALUES (?,?,?)");
        static thread_local SQ    s("SELECT id FROM Atoms WHERE k=?");
        auto i_af   = i.af();
        auto s_af   = s.af();
        i.bind(0, dk);
        i.bind(1, d.id);
        i.bind(2, ck);
        
        if(i.exec(false)){
            if(wasCreated)
                *wasCreated = true;
            return Atom{i.lastInsertIdU64()};
        } else {
            s.bind(0, dk);
            if(s.exec() && s.next())
                return Atom{s.valueU64(0)};
            yError() << "Unable to get the atom from the database: " << dk;
            return Atom();
        }
    }
    
    //Document            document(Atom a)
    //{
        //static thread_local SQ s("SELECT doc FROM Atoms WHERE id=?");
        //auto s_af = s.af();
        //s.bind(0, a.id);
        //if(s.exec() && s.next())
            //return Document{s.valueU64(0)};
        //return Document();
    //}
    
    namespace {
        DocVec  documents_sorted(Atom a)
        {
            static thread_local SQ  s("SELECT doc FROM ADocuments INNER JOIN Documents ON ADocuments.doc=Documents.id WHERE id=? ORDER BY Documents.k");
            return s.vec<Document>(a.id);
        }
        
        DocVec  documents_unsorted(Atom a)
        {
            static thread_local SQ  s("SELECT doc FROM ADocuments WHERE id=?");
            return s.vec<Document>(a.id);
        }
        
    }
    
    DocVec        documents(Atom a, Sorted sorted)
    {
        return sorted ? documents_sorted(a) : documents_unsorted(a);
    }
    
    size_t                  documents_count(Atom a)
    {
        static thread_local SQ s("SELECT COUNT(1) FROM ADocuments WHERE id=?");
        return s.size(a.id);
    }

   bool                exists(Atom a)
    {
        return exists_atom(a.id);
    }
    
   
    bool                exists_atom(uint64_t i)
    {
        static thread_local SQ s("SELECT 1 FROM Atoms WHERE id=?");
        return s.present(i);
    }

    
   
    Image               icon(Atom a) 
    {
        static thread_local SQ s("SELECT icon FROM Atoms WHERE id=?");
        return s.as<Image>(a.id);
    }
    
    //AtomVec            inbound(Atom);

    Atom::Info          info(Atom a)
    {
        Atom::Info    ret;
        
        static thread_local SQ s("SELECT k, abbr, brief, name FROM Atoms WHERE id=?");
        auto s_af = s.af();
        s.bind(0, a.id);
        if(s.exec() && s.next()){
            ret.key         = s.valueString(0);
            ret.abbr        = s.valueString(1);
            //ret.doc         = Document(s.valueU64(2));
            ret.brief       = s.valueString(2);
            ret.name        = s.valueString(3);
        }
        return ret;
    }
    
    
    bool                is(Atom a, Class c)
    {
        static thread_local SQ    s("SELECT 1 FROM AClasses WHERE atom=? AND class=?");
        return s.present(a.id, c.id);
    }
    
    //bool                is_all(Atom a, std::initializer_list<Class>cs)
    //{
        //for(Class c : cs)
            //if(!is(a, c))
                //return false;
        //return true;
    //}
    
    //bool                is_any(Atom a, std::initializer_list<Class>cs)
    //{
        //for(Class c : cs)
            //if(is(a, c))
                //return true;
        //return false;
    //}
    
 
    String             key(Atom a)
    {
        static thread_local SQ s("SELECT k FROM Atoms WHERE id=?");
        return s.str(a.id);
    }
    
    String             label(Atom a)
    {
        String n   = name(a);
        if(n.empty())
            n       = key(a);
        return n;
    }
    
    
    //Leaf                    leaf(Atom

    String             name(Atom a)
    {
        static thread_local SQ s("SELECT name FROM Atoms WHERE id=?");
        return s.str(a.id);
    }

    
    NKI                 nki(Atom a, bool autoKey)
    {
        static thread_local SQ    s("SELECT name,icon,k FROM Atoms WHERE id=?");
        auto s_af = s.af();
        s.bind(0, a.id);
        if(s.exec() && s.next()){
            NKI  ret;
            ret.name    = s.valueString(0);
            ret.icon    = Image{s.valueU64(1)};
            ret.key     = s.valueString(2);
            if(autoKey && ret.name.empty())
                ret.name    = ret.key;
            return ret;
        }
        return NKI();
    }
    

    //AtomVec            outbound(Atom);

    Atom                parent(Atom a)
    {
        static thread_local SQ s("SELECT parent FROM Atoms WHERE id=?");
        return s.as<Atom>(a.id);
    }
    
    
    QList<QVariant>         qvar_list(const Set<Atom>&all)
    {
        QVariantList    ret;
        for(const Atom t : all)
            ret << (quint64) t.id;
        return ret;
    }

    String             skey(Atom a) 
    {
        static thread_local SQ s("SELECT sk FROM Atoms WHERE id=?");
        return s.str(a.id);
    }

    namespace {
        TagVec          tags_sorted(Atom a)
        {
            static thread_local SQ s("SELECT DISTINCT tag FROM ATags INNER JOIN Tags ON ATags.tag=Tags.id WHERE atom=? ORDER BY Tags.k");
            return s.vec<Tag>(a.id);
        }

        TagVec          tags_unsorted(Atom a)
        {
            static thread_local SQ s("SELECT DISTINCT tag FROM ATags WHERE atom=?");
            return s.vec<Tag>(a.id);
        }
    }
    
    TagVec              tags(Atom a, Sorted sorted)
    {
        return sorted ? tags_sorted(a) : tags_unsorted(a);
    }
    
    namespace {
        TagVec          tags_sorted(Atom a, Document d)
        {
            static thread_local SQ s("SELECT tag FROM ATags INNER JOIN Tags ON ATags.tag=Tags.id WHERE atom=? AND doc=? ORDER BY Tags.k");
            return s.vec<Tag>(a.id,d.id);
        }

        TagVec          tags_unsorted(Atom a, Document d)
        {
            static thread_local SQ s("SELECT tag FROM ATags WHERE atom=? AND doc=?");
            return s.vec<Tag>(a.id,d.id);
        }
    }
    

    TagVec              tags(Atom a, Document d, Sorted sorted)
    {
        return sorted ? tags_sorted(a,d) : tags_unsorted(a,d);
    }

    bool                tagged(Atom a, Tag t)
    {
        static thread_local SQ s("SELECT 1 FROM ATags WHERE atom=? AND tag=? LIMIT 1");
        return s.present(a.id, t.id);
    }
    
    size_t              tags_count(Atom a)
    {
        static thread_local SQ s("SELECT COUNT(DISTINCT tag) FROM ATags WHERE atom=?");
        return s.size(a.id);
    }

    size_t              tags_count(Atom a, Document d)
    {
        static thread_local SQ s("SELECT COUNT(1) FROM ATags WHERE atom=? AND doc=?");
        return s.size(a.id, d.id);
    }

    String             title(Atom a)
    {
        return label(a);
    }
}
