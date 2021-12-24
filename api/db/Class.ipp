////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

bool Class::less_key(Class a, Class b)
{
    return is_less_igCase(cdb::key(a), cdb::key(b));
}

bool Class::less_label(Class a, Class b)
{
    return is_less_igCase(cdb::label(a), cdb::label(b));
}

bool Class::less_name(Class a, Class b)
{
    return is_less_igCase(cdb::name(a), cdb::name(b));
}


namespace cdb {

    StringSet              aliases(Field f)
    {
        static thread_local SQ s("SELECT alias FROM FAlias WHERE field=?");
        return s.sset(f.id);
    }

    namespace {
        ClassVec    all_classes_sorted()
        {
            static thread_local SQ    s("SELECT id FROM Classes ORDER BY K");
            return s.vec<Class>();
        }
        
        ClassVec    all_classes_unsorted()
        {
            static thread_local SQ    s("SELECT id FROM Classes");
            return s.vec<Class>();
        }
    }


    ClassVec       all_classes(Sorted sorted)
    {
        return sorted ? all_classes_sorted() : all_classes_unsorted();
    }

    
    size_t              all_classes_count()
    {
        static thread_local SQ    s("SELECT COUNT(1) FROM Classes");
        return s.size();
    }
    
    namespace {
        FieldVec    all_fields_sorted()
        {
            static thread_local SQ s("SELECT id FROM Fields ORDER BY k");
            return s.vec<Field>();
        }
        
        FieldVec    all_fields_unsorted()
        {
            static thread_local SQ s("SELECT id FROM Fields");
            return s.vec<Field>();
        }
    }
    
    FieldVec           all_fields(Sorted sorted)
    {
        return sorted ? all_fields_sorted() : all_fields_unsorted();
    }
    
    size_t              all_fields_count()
    {
        static thread_local SQ s("SELECT COUNT(1) FROM Fields");
        return s.size();
    }
    
    
    
    String             brief(Class c)
    {
        static thread_local SQ    s("SELECT brief FROM Classes WHERE id=?");
        return s.str(c.id);
    }
    
    String             brief(Field f)
    {
        static thread_local SQ s("SELECT brief FROM Fields WHERE id=?");
        return s.str(f.id);
    }
    
    Class               class_(Field f)
    {
        static thread_local SQ s("SELECT class FROM Fields WHERE id=?");
        return s.as<Class>(f.id);
    }

    Class               class_(uint64_t i)
    {
        return exists_class(i) ? Class{i} : Class();
    }
    
    Class               class_(const String& k)
    {
        static thread_local SQ s("SELECT id FROM Classes WHERE k=?");
        return s.as<Class>(k);
    }

    ClassFile::Shared        class_doc(Fragment f, bool fAllowEmpty)
    {
        if(!f)
            return ClassFile::Shared();
            
        auto          ch  = frag_bytes(f);
        if(ch.empty())
            return fAllowEmpty ? std::make_shared<ClassFile>() : ClassFile::Shared();
            
        ClassFile::Shared  td = std::make_shared<ClassFile>();
        auto fp = path(f);
        if(!td->load(std::move(ch), fp)){
            yError() << "Unable to read " << fp;
            return ClassFile::Shared();
        }
        td -> set_file(fp);
        return td;
    }
    
    namespace {
        ClassVec       classes_sorted(Field f)
        {
            static thread_local SQ s("SELECT class FROM CFields INNER JOIN Classes ON CFields.class=Classes.id WHERE field=? ORDER BY Classes.k");
            return s.vec<Class>(f.id);
        }

        ClassVec       classes_unsorted(Field f)
        {
            static thread_local SQ s("SELECT class FROM CFields WHERE field=?");
            return s.vec<Class>(f.id);
        }
    }
    
    
    ClassVec       classes(Field f, Sorted sorted)
    {
        return sorted ? classes_sorted(f) : classes_unsorted(f);
    }

    ClassVec       classes(const StringSet& sset)
    {
        ClassVec   ret;
        for(String s : sset)
            ret << db_class(s.qString());
        return ret;
    }
    
    StringSet          data_types(Field f)
    {
        static thread_local SQ    s("SELECT type FROM FDataTypes WHERE field=?");
        return s.sset(f.id);
    }

    Class               db_class(const String&k, bool *wasCreated)
    {
        return db_class(db_document(classes_folder(), k+".cls"), wasCreated);
    }
    
    Class               db_class(Document doc, bool *wasCreated)
    {
        if(wasCreated)
            *wasCreated = false;
        if(folder(doc) != classes_folder())
            return Class{};
        String k   = base_key(doc);
        if(k.empty())
            return Class();
        
        static thread_local SQ    i("INSERT OR FAIL INTO Classes (k,id) VALUES (?,?)");
        auto i_lk   = i.af();

        i.bind(0, k);
        i.bind(1, doc.id);
        if(i.exec(false)){
            if(wasCreated)
                *wasCreated = true;
            return Class{doc.id};
        } else if(exists_class(doc.id)){
            return Class{doc.id};
        } else {
            yError() << "Unable to get the class from the database: " << k;
            return Class();
        }
    }

    ClassVec            db_classes(const StringSet& all)
    {
        ClassVec   ret;
        for(const String& s: all)
            ret << db_class(s);
        return ret;
    }
    

    Field               db_field(Class c, const String&k, bool *wasCreated)
    {
        if(wasCreated)
            *wasCreated = false;
        if(k.empty())
            return Field{};
            
        static thread_local SQ i("INSERT INTO Fields (class, k) VALUES (?,?)");
        static thread_local SQ s("SELECT id FROM Fields WHERE class=? AND k=?");
        auto i_af = i.af();
        auto s_af = s.af();
        i.bind(0, c.id);
        i.bind(1, k);
        if(i.exec()){
            if(wasCreated)
                *wasCreated = true;
            return Field(i.lastInsertIdU64());
        } else {
            s.bind(0, c.id);
            s.bind(1, k);
            if(s.exec() && s.next())
                return Field(s.valueU64(0));
            
            yInfo() << "Unable to create field ID!";
            return Field();
        }
    }
    
    namespace {
        ClassVec           def_derived_sorted(Class c)
        {   
            static thread_local SQ s("SELECT class FROM CDependsDef INNER JOIN Classes ON CDependsDef.class=Classes.id WHERE base=? ORDER BY Classes.k");
            return s.vec<Class>();
        }

        ClassVec           def_derived_unsorted(Class c)
        {   
            static thread_local SQ s("SELECT class FROM CDependsDef WHERE base=?");
            return s.vec<Class>();
        }
    }
    
    ClassVec                def_derived(Class c, Sorted sorted)
    {
        return sorted ? def_derived_sorted(c) : def_derived_unsorted(c);
    }

    namespace {
        FieldVec           def_fields_sorted(Class c)
        {
            static thread_local SQ s("SELECT field FROM CFieldDef INNER JOIN Fields ON CFieldsDef.field=Fields.id WHERE class=? ORDER BY Fields.k");
            return s.vec<Field>(c.id);
        }

        FieldVec           def_fields_unsorted(Class c)
        {
            static thread_local SQ s("SELECT field FROM CFieldDef WHERE class=?");
            return s.vec<Field>(c.id);
        }
    }
    
    FieldVec                def_fields(Class c, Sorted sorted)
    {
        return sorted ? def_fields_sorted(c) : def_fields_unsorted(c);
    }
    
    namespace {
        ClassVec        def_reverse_sorted(Class c)
        {
            static thread_local SQ s("SELECT reverse FROM CReverseDef INNER JOIN Classes ON CReverseDef.reverse=Classes.id WHERE class=? ORDER BY Classes.k");
            return s.vec<Class>(c.id);
        }

        ClassVec        def_reverse_unsorted(Class c)
        {
            static thread_local SQ s("SELECT reverse FROM CReverseDef WHERE class=?");
            return s.vec<Class>(c.id);
        }
    }
    
    ClassVec           def_reverse(Class c, Sorted sorted)
    {   
        return sorted ? def_reverse_sorted(c) : def_reverse_unsorted(c);
    }
    
    namespace {
        ClassVec        def_source_sorted(Class c)
        {
            static thread_local SQ s("SELECT source FROM CSourceDef INNER JOIN Classes ON CSourceDef.source=Classes.id WHERE class=? ORDER BY Classes.k");
            return s.vec<Class>(c.id);
        }

        ClassVec        def_source_unsorted(Class c)
        {
            static thread_local SQ s("SELECT source FROM CSourceDef WHERE class=?");
            return s.vec<Class>(c.id);
        }
    }

    ClassVec           def_source(Class c, Sorted sorted)
    {
        return sorted ? def_source_sorted(c) : def_source_unsorted(c);
    }
    
    namespace {
        ClassVec        def_target_sorted(Class c)
        {
            static thread_local SQ s("SELECT target FROM CTargetDef INNER JOIN Classes ON CTargetDef.target=Classes.id WHERE class=? ORDER BY Classes.k");
            return s.vec<Class>(c.id);
        }

        ClassVec        def_target_unsorted(Class c)
        {
            static thread_local SQ s("SELECT target FROM CTargetDef WHERE class=?");
            return s.vec<Class>(c.id);
        }
    }
    
    
    ClassVec           def_target(Class c, Sorted sorted)
    {
        return sorted ? def_target_sorted(c) : def_target_unsorted(c);
    }

    namespace {
        ClassVec        def_use_sorted(Class c)
        {
            static thread_local SQ s("SELECT base FROM CDependsDef INNER JOIN Classes ON CDependsDef.base=Classes.id WHERE class=? ORDER BY Classes.k");
            return s.vec<Class>(c.id);
        }

        ClassVec        def_use_unsorted(Class c)
        {
            static thread_local SQ s("SELECT base FROM CDependsDef WHERE class=?");
            return s.vec<Class>(c.id);
        }
    }

    ClassVec           def_use(Class c, Sorted sorted)
    {
        return sorted ? def_use_sorted(c) : def_use_unsorted(c);
    }
    
    Graph               dep_graph(Class c)
    {   
        static thread_local SQ    s("SELECT deps FROM Classes WHERE id=?");
        return s.as<Graph>(c.id);
    }
    
    namespace {
        ClassVec        dependents_sorted(Class c)
        {
            static thread_local SQ    s("SELECT class FROM CDepends INNER JOIN Classes ON CDepends.class=Classes.id WHERE base=? ORDER BY Classes.k");
            return s.vec<Class>(c.id);
        }
        
        ClassVec        dependents_unsorted(Class c)
        {
            static thread_local SQ    s("SELECT class FROM CDepends WHERE base=?");
            return s.vec<Class>(c.id);
        }
    }
    

    ClassVec       dependents(Class c, Sorted sorted)
    {
        return sorted ? dependents_sorted(c) : dependents_unsorted(c);
    }

    Folder              detail_folder(Class c)
    {
        return db_folder( classes_folder(), key(c));
    }


    Document            document(Class c)
    {
        return exists(c) ? Document{c.id}  : Document{};
    }
    
    Document                document(Field f)
    {
        return document(class_(f));
    }

    bool                edge(Class c)
    {
        static thread_local SQ    s("SELECT edge FROM Classes WHERE id=?");
        return s.boolean(c.id);
    }
    
    namespace {
        ClassVec    edges_in_sorted(Class c)
        {
            static thread_local SQ    s("SELECT class FROM CTargets INNER JOIN Classes ON CTargets.class=Classes.id WHERE target=? ORDER BY Classes.K");
            return s.vec<Class>(c.id);
        }

        ClassVec    edges_in_unsorted(Class c)
        {
            static thread_local SQ    s("SELECT class FROM CTargets WHERE target=?");
            return s.vec<Class>(c.id);
        }
    }
    

    ClassVec      edges_in(Class c, Sorted sorted)
    {
        return sorted ? edges_in_sorted(c) : edges_in_unsorted(c);
    }
    
    namespace {
        ClassVec    edges_out_sorted(Class c)
        {
            static thread_local SQ    s("SELECT class FROM CSources INNER JOIN Classes ON CSources.class=Classes.id WHERE source=? ORDER BY Classes.K");
            return s.vec<Class>(c.id);
        }
        
        ClassVec    edges_out_unsorted(Class c)
        {
            static thread_local SQ    s("SELECT class FROM CSources WHERE source=?");
            return s.vec<Class>(c.id);
        }
    }
    
    ClassVec      edges_out(Class c, Sorted sorted)
    {
        return sorted ? edges_out_sorted(c) : edges_out_unsorted(c);
    }

    bool                exists(Class c)
    {
        return exists_class(c.id);
    }
    
    
    bool                exists(Field f)
    {
        return exists_field(f.id);
    }
    

    bool                exists_class(uint64_t i)
    {
        static thread_local SQ s("SELECT 1 FROM Classes WHERE id=?");
        return s.present(i);
    }


    bool                exists_field(uint64_t i)
    {
        static thread_local SQ s("SELECT 1 FROM Fields WHERE id=?");
        return s.present(i);
    }

    Field               field(uint64_t i)
    {
        return exists_field(i) ? Field{i} : Field{};
    }
    
    Field               field(Class c, const String&k)
    {
        static thread_local SQ s("SELECT id FROM Fields WHERE class=? AND k=?");
        return s.as<Field>(c.id, k);
    }
    
    namespace {
        FieldVec    fields_sorted(Class c)
        {
            static thread_local SQ    s("SELECT field FROM CFields INNER JOIN Fields ON CFields.field=Fields.id WHERE class=? ORDER BY Fields.K");
            return s.vec<Field>(c.id);
        }

        FieldVec    fields_unsorted(Class c)
        {
            static thread_local SQ    s("SELECT field FROM CFields WHERE class=?");
            return s.vec<Field>(c.id);
        }
    }
    

    FieldVec       fields(Class c, Sorted sorted)
    {
        return sorted ? fields_sorted(c) : fields_unsorted(c);
    }
    
    size_t          fields_count(Class c)
    {
        static thread_local SQ    s("SELECT COUNT(1) FROM CFields WHERE class=?");
        return s.size(c.id);
    }
    
    
    Image               icon(Class c)
    {
        static thread_local SQ    s("SELECT icon FROM Classes WHERE id=? LIMIT 1");
        return s.as<Image>(c.id);
    }
    
    
    Image               icon(Field f)
    {
        static thread_local SQ    s("SELECT icon FROM Fields WHERE id=?");
        return s.as<Image>(f.id);
    }
    
    namespace {
        ClassVec        inbound_sorted(Class c)
        {
            static thread_local SQ s("SELECT class FROM CTargets INNER JOIN Classes ON CTargets.cleass=Classes.id WHERE target=? ORDER BY Classes.K");
            return s.vec<Class>(c.id);
        }
        
        ClassVec        inbound_unsorted(Class c)
        {
            static thread_local SQ s("SELECT class FROM CTargets WHERE target=?");
            return s.vec<Class>(c.id);
        }
    }
    
    ClassVec           inbound(Class c, Sorted sorted)
    {
        return sorted ? inbound_sorted(c) : inbound_unsorted(c);
    }
    
    Class::Info         info(Class c, bool autoKey)
    {
        Class::Info    ret;
        static thread_local SQ    s("SELECT k, edge, name, plural, brief, icon, deps FROM Classes WHERE id=?");
        auto s_af = s.af();
        s.bind(0, c.id);
        if(s.exec() && s.next()){
            ret.key         = s.valueString(0);
            ret.doc         = Document(c.id);
            ret.edge        = s.valueAs<bool>(1);
            ret.name        = s.valueString(2);
            ret.plural      = s.valueString(3);
            ret.brief       = s.valueString(4);
            ret.icon        = Image(s.valueU64(5));
            ret.deps        = Graph(s.valueU64(6));
            if(autoKey && ret.name.empty())
                ret.name    = ret.key;
        }
        return ret;
    }
    

    
    Field::Info         info(Field f, bool autoKey)
    {
        Field::Info        ret;
        static thread_local SQ s("SELECT k, class, name, pkey, plural, brief FROM Fields WHERE id=?");
        auto s_af = s.af();
        s.bind(0, f.id);
        if(s.exec() && s.next()){
            ret.key     = s.valueString(0);
            ret.class_  = Class(s.valueU64(1));
            ret.name    = s.valueString(2);
            ret.pkey    = s.valueString(3);
            ret.plural  = s.valueString(4);
            ret.brief   = s.valueString(5);
            if(autoKey && ret.name.empty())
                ret.name    = ret.key;
        }
        return ret;
    }
    
    bool                is(Class d, Class b)
    {
        static thread_local SQ    s("SELECT 1 FROM CDepends WHERE class=? AND base=?");
        return s.present(d.id, b.id);
    }
    
    //bool                is_all(Class d, std::initializer_list<Class> b)
    //{
        //for(Class c : b)
            //if(!is(d, c))
                //return false;
        //return true;
    //}

    //bool                is_any(Class d, std::initializer_list<Class> b)
    //{
        //for(Class c : b)
            //if(is(d, c))
                //return true;
        //return false;
    //}


    
    String             key(Class c)
    {
        static thread_local SQ    s("SELECT k FROM Classes WHERE id=? LIMIT 1");
        return s.str(c.id);
    }
    
    
    String             key(Field f)
    {
        static thread_local SQ s("SELECT k FROM Fields WHERE id=?");
        return s.str(f.id);
    }
    
    
    String             label(Class c)
    {
        String     n   = name(c);
        return n.empty() ? key(c) : n;
    }

    String             label(Field f)
    {
        String     n = name(f);
        return n.empty() ? key(f) : n;
    }
    

    Class                make_class(const String&k, const Root* rt)
    {
        if(!rt)
            rt      = wksp::root_first(DataRole::Classes);
        if(!rt){
            yError() << "No root specified to create the class in!";
            return Class{};
        }
        
        Document        doc = db_document(classes_folder(), k+".cls");
        bool            was = false;
        Class           c   = db_class(doc, &was);
        if(!was)
            return c;
            
        if(fragments_count(doc))
            return c;
        ClassFile::Shared    td  = writable(c, rt);
        td -> name      = k;
        td -> plural    = k + 's';
        td -> save();
        return c;
    }
    

    ClassData::Shared            merged(Class c, unsigned int opts)
    {
        ClassData::Shared        ret = std::make_shared<ClassData>();;
        for(auto& i : reads(c)){
            if(opts & IsUpdate)
                update(i.first);
            ret->merge(*(i.second), static_cast<bool>(opts&Override));
        }
        return ret;
    }
    


    String             name(Class c)
    {
        static thread_local SQ    s("SELECT name FROM Classes WHERE id=?");
        return s.str(c.id);
    }

    
    String             name(Field f)
    {
        static thread_local SQ    s("SELECT name FROM Fields WHERE id=?");
        return s.str(f.id);
    }

    NKI                 nki(Class c, bool autoKey)
    {
        static thread_local SQ    s("SELECT name,icon,k FROM Classes WHERE id=?");
        auto s_af = s.af();
        s.bind(0, c.id);
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
    
    
    NKI                 nki(Field f, bool autoKey)
    {
        static thread_local SQ    s("SELECT name,icon,k FROM Fields WHERE id=?");
        auto s_af = s.af();
        s.bind(0, f.id);
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
    
    namespace {
        ClassVec    outbound_sorted(Class c)
        {
            static thread_local SQ s("SELECT class FROM CSources INNER JOIN Classes ON CSources.class=Classes.id WHERE source=? ORDER BY Classes.K");
            return s.vec<Class>(c.id);
        }

        ClassVec    outbound_unsorted(Class c)
        {
            static thread_local SQ s("SELECT class FROM CSources WHERE source=?");
            return s.vec<Class>(c.id);
        }
    }

    ClassVec           outbound(Class c, Sorted sorted)
    {
        return sorted ? outbound_sorted(c) : outbound_unsorted(c);
    }

    
    Class               parent(Field f)
    {
        return class_(f);
    }

    
    String             pkey(Field f)
    {
        static thread_local SQ s("SELECT pkey FROM Fields WHERE id=?");
        return s.str(f.id);
    }
    
    String             plural(Class c)
    {
        static thread_local SQ    s("SELECT plural FROM Classes WHERE id=?");
        return s.str(c.id);
    }
    
    String             plural(Field f)
    {
        static thread_local SQ s("SELECT plural FROM Fields WHERE id=?");
        return s.str(f.id);
    }
    
    
    QList<QVariant>         qvar_list(const Set<Class>&all)
    {
        QVariantList    ret;
        for(const Class t : all)
            ret << (quint64) t.id;
        return ret;
    }
    
    QList<QVariant>         qvar_list(const Set<Field>&all)
    {
        QVariantList    ret;
        for(const Field t : all)
            ret << (quint64) t.id;
        return ret;
    }


    ClassFile::Shared        read(Class c, const Root*rt)
    {
        return class_doc(fragment(document(c), rt));
    }

    Vector<Class::FragDoc>    reads(Class c)
    {
        Vector<Class::FragDoc>  ret;
        for(Fragment f : fragments(document(c), DataRole::Classes)){
            ClassFile::Shared    p   = class_doc(f);
            if(p)
                ret << Class::FragDoc(f, p);
        }
        return ret;
    }

    Vector<Class::FragDoc>  reads(Class c, class Root*rt)
    {
        Vector<Class::FragDoc>  ret;
        for(Fragment f : fragments(document(c), rt)){
            ClassFile::Shared    p   = class_doc(f);
            if(p)
                ret << Class::FragDoc(f, p);
        }
        return ret;
    }
    
    namespace {
        ClassVec    reverses_sorted(Class c)
        {
            static thread_local SQ    s("SELECT reverse FROM CReverses INNER JOIN Classes ON CReverses.reverse=Classes.id WHERE class=? ORDER BY Classes.K");
            return s.vec<Class>(c.id);
        }

        ClassVec    reverses_unsorted(Class c)
        {
            static thread_local SQ    s("SELECT reverse FROM CReverses WHERE class=?");
            return s.vec<Class>(c.id);
        }
    }
    
    
    ClassVec       reverses(Class c, Sorted sorted)
    {
        return sorted ? reverses_sorted(c) : reverses_unsorted(c);
    }
    
    namespace {
        ClassVec    sources_sorted(Class c)
        {
            static thread_local SQ    s("SELECT source FROM CSources INNER JOIN Classes ON CSources.source=Classes.id WHERE class=? ORDER BY Classes.K");
            return s.vec<Class>(c.id);
        }

        ClassVec    sources_unsorted(Class c)
        {
            static thread_local SQ    s("SELECT source FROM CSources WHERE class=?");
            return s.vec<Class>(c.id);
        }
    }

    ClassVec  sources(Class c, Sorted sorted)
    {
        return sorted ? sources_sorted(c) : sources_unsorted(c);
    }

    size_t  sources_count(Class c)
    {
        static thread_local SQ    s("SELECT COUNT(1) FROM CSources WHERE class=?");
        return s.size(c.id);
    }

    bool  tagged(Class c, Tag t)
    {
        static thread_local SQ s("SELECT 1 FROM CTags WHERE class=? AND tag=?");
        return s.present(c.id, t.id);
    }

    namespace {
        TagVec  tags_sorted(Class c)
        {
            static thread_local SQ s("SELECT tag FROM CTags INNER JOIN Tags ON CTags.tag=Tags.id WHERE class=? ORDER BY Tags.K");
            return s.vec<Tag>(c.id);
        }

        TagVec  tags_unsorted(Class c)
        {
            static thread_local SQ s("SELECT tag FROM CTags WHERE class=?");
            return s.vec<Tag>(c.id);
        }
    }

    TagVec  tags(Class c, Sorted sorted)
    {
        return sorted ? tags_sorted(c) : tags_unsorted(c);
    }
    
    namespace {
        TagVec  tags_sorted(Field f)
        {
            static thread_local SQ s("SELECT tag FROM FTags INNER JOIN Tags ON FTags.tag=Tags.id WHERE field=? ORDER BY Tags.K");
            return s.vec<Tag>(f.id);
        }
        
        TagVec  tags_unsorted(Field f)
        {
            static thread_local SQ s("SELECT tag FROM FTags WHERE field=?");
            return s.vec<Tag>(f.id);
        }
    }
    
    TagVec  tags(Field f, Sorted sorted)
    {
        return sorted ? tags_sorted(f) : tags_unsorted(f);
    }

    
    size_t  tags_count(Class c)
    {
        static thread_local SQ s("SELECT COUNT(1) FROM CTags WHERE class=?");
        return s.size(c.id);
    }
    
    size_t  tags_count(Field f)
    {
        static thread_local SQ s("SELECT COUNT(1) FROM FTags WHERE field=?");
        return s.size(f.id);
    }


    namespace {
        ClassVec    targets_sorted(Class c)
        {
            static thread_local SQ    s("SELECT target FROM CTargets INNER JOIN Classes ON CTargets.target=Classes.id WHERE class=? ORDER BY Classes.k");
            return s.vec<Class>(c.id);
        }

        ClassVec    targets_unsorted(Class c)
        {
            static thread_local SQ    s("SELECT target FROM CTargets WHERE class=?");
            return s.vec<Class>(c.id);
        }
    }
    
    ClassVec       targets(Class c, Sorted sorted)
    {
        return sorted ? targets_sorted(c) : targets_unsorted(c);
    }

    size_t          targets_count(Class c)
    {
        static thread_local SQ    s("SELECT COUNT(1) FROM CTargets WHERE class=?");
        return s.size(c.id);
    }
    
    namespace {
        ClassVec    uses_sorted(Class c)
        {
            static thread_local SQ    s("SELECT base FROM CDepends INNER JOIN Classes ON CDepends.base=Classes.id WHERE class=? ORDER BY Classes.k");
            return s.vec<Class>(c.id);
        }

        ClassVec    uses_unsorted(Class c)
        {
            static thread_local SQ    s("SELECT base FROM CDepends WHERE class=?");
            return s.vec<Class>(c.id);
        }
    }
    

    ClassVec   uses(Class c, Sorted sorted)
    {
        return sorted ? uses_sorted(c) : uses_unsorted(c);
    }
    
    size_t              uses_count(Class c)
    {
        static thread_local SQ s("SELECT COUNT(1) FROM CDepends WHERE class=?");
        return s.size(c.id);
    }
    
    ClassFile::Shared        writable(Class c, const Root* rt)
    {
        Document    d   = document(c);
        if(!d)
            return ClassFile::Shared();
        Fragment    f   = fragment(d);
        if(f)
            return class_doc(f, true);
        Folder      fo  = folder(d);
        if((fo != cdb::top_folder()) && !exists(fo, rt))
            rt -> make_path(key(fo));
        ClassFile::Shared ptr  = std::make_shared<ClassFile>();
        ptr -> set_file( rt -> resolve(key(d)));
        return ptr;
    }


}
