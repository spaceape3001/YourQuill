////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace cdb {
    namespace {
        TagVec     all_tags_sorted()
        {
            static thread_local SQ    s("SELECT id FROM Tags ORDER BY k");
            return s.vec<Tag>();
        }
        
        TagVec     all_tags_unsorted()
        {
            static thread_local SQ    s("SELECT id FROM Tags");
            return s.vec<Tag>();
        }
    }

    TagVec         all_tags(Sorted sorted)
    {
        return sorted ? all_tags_sorted() : all_tags_unsorted();
    }
    
    size_t              all_tags_count()
    {
        static thread_local SQ s("SELECT COUNT(1) FROM Tags");
        return s.size();
    }

    String             brief(Tag t)
    {
        static thread_local SQ    s("SELECT brief FROM Tags WHERE id=?");
        return s.str(t.id);
    }

    Tag                 db_tag(Document doc, bool* wasCreated)
    {
        if(wasCreated)
            *wasCreated = false;
        if(folder(doc) != tags_folder()){
            yError() << "Rejecting tag " << key(doc) << " due to not being in the '.tags' folder";        
            return Tag{};
        }
        String k   = base_key(doc);
        if(k.empty())
            return Tag();
        
        static thread_local SQ    i("INSERT OR FAIL INTO Tags (k,id) VALUES (?,?)");
        auto i_lk   = i.af();
        
        i.bind(0, k);
        i.bind(1, doc.id);
        if(i.exec(false)){
            if(wasCreated)
                *wasCreated = true;
            return Tag{doc.id};
        } else if(exists_tag(doc.id)){
            return Tag{doc.id};
        } else {
            yError() << "Unable to get the tag from the database: " << k;
            return Tag{};
        }
    }
    
    Tag                 db_tag(Fragment f, bool* wasCreated)
    {
        return db_tag(document(f),wasCreated);
    }

    Tag                 db_tag(const String&k, bool* wasCreated)
    {
        return db_tag(db_document(tags_folder(), k+".tag"), wasCreated);
    }
    
    TagVec             db_tags(const StringSet&all)
    {
        TagVec     ret;
        for(const String& s:all)
            ret << db_tag(s);
        return ret;
    }
    

    Document            document(Tag t)
    {
        return exists(t) ? Document{t.id} : Document{};
    }    


    bool                exists(Tag t)
    {
        return exists_tag(t.id);
    }


    bool                exists_tag(uint64_t i)
    {
        static thread_local SQ s("SELECT 1 FROM Tags WHERE id=? LIMIT 1");
        return s.present(i);
    }

    Image               icon(Tag t)
    {
        static thread_local SQ    s("SELECT icon FROM Tags WHERE id=? LIMIT 1");
        return s.as<Image>(t.id);
    }
    
    
    Tag::Info           info(Tag t, bool autoKey)
    {
        Tag::Info    ret;
        static thread_local SQ    s("SELECT brief,k,name FROM Tags WHERE id=?");
        auto s_af = s.af();
        s.bind(0, t.id);
        if(s.exec() && s.next()){
            ret.brief       = s.valueString(0);
            ret.doc         = Document(t.id);
            ret.key         = s.valueString(1);
            ret.name        = s.valueString(3);
            if(autoKey && ret.name.empty())
                ret.name    = ret.key;
        }
        return ret;
    }
    

    String             key(Tag t)
    {
        static thread_local SQ    s("SELECT k FROM Tags WHERE id=?");
        return s.str(t.id);
    }
    
    String             label(Tag t)
    {
        String     n   = name(t);
        return n.empty() ? key(t) : n;
    }
    
    Leaf                leaf(Tag t)
    {
        static thread_local SQ    s("SELECT leaf FROM Tags WHERE id=?");
        return s.as<Leaf>(t.id);
    }

    Tag                     make_tag(const String& k, const Root* rt)
    {
        if(!rt)
            rt  = wksp::root_first(DataRole::Tags);
        if(!rt){
            yError() << "No root specified to create the tag in!";
            return Tag{};
        }
        
        Document    doc = db_document(tags_folder(), k + ".tag");
        bool            was = false;
        Tag         t   = db_tag(doc, &was);
        if(!was)
            return t;
        if(fragments_count(doc))
            return t;
        TagFile::Shared td  = write(t, rt);
        td -> name  = k;
        td -> save();
        return t;
    }

    SharedTagData   merged(Tag t, unsigned int opts)
    {
        SharedTagData  ret = std::make_shared<TagData>();
        for(auto& i :reads(t)){
            if(opts & IsUpdate)
                update(i.first);
            ret -> merge(*(i.second), static_cast<bool>(opts&Override));
        }
        return ret;
    }
    
    String             name(Tag t)
    {
        static thread_local SQ    s("SELECT name FROM Tags WHERE id=?");
        return s.str(t.id);
    }
    
    
    NKI                 nki(Tag t, bool autoKey)
    {
        static thread_local SQ    s("SELECT name,icon,k FROM Tags WHERE id=?");
        auto s_af = s.af();
        s.bind(0, t.id);
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
    
    QList<QVariant>         qvar_list(const Set<Tag>&all)
    {
        QVariantList    ret;
        for(const Tag t : all)
            ret << (quint64) t.id;
        return ret;
    }

    TagFile::Shared          read(Tag t, const Root* rt)
    {
        return tag_doc(fragment(document(t), rt));
    }

    
    Vector<Tag::FragDoc>      reads(Tag t)
    {
        Vector<Tag::FragDoc>  ret;
        for(Fragment f : fragments(document(t), DataRole::Tags)){
            TagFile::Shared  p   = tag_doc(f);
            if(p)
                ret << Tag::FragDoc(f,p);
        }
        return ret;
    }

    Vector<Tag::FragDoc>    reads(Tag t, class Root* rt)
    {
        Vector<Tag::FragDoc>  ret;
        for(Fragment f : fragments(document(t), rt)){
            TagFile::Shared  p   = tag_doc(f);
            if(p)
                ret << Tag::FragDoc(f,p);
        }
        return ret;
    }
    
    bool                set_brief(Tag t, const String& k, class Root* rt)
    {
        auto tf = write(t, rt);
        if(!tf)
            return false;
        tf -> brief     = k;
        return tf -> save();
    }
    
    bool                set_name(Tag t, const String& k, class Root* rt)
    {
        auto tf = write(t, rt);
        if(!tf)
            return false;
        tf -> name     = k;
        return tf -> save();
    }
    
    bool                set_notes(Tag t, const String& k, class Root* rt)
    {
        auto tf = write(t, rt);
        if(!tf)
            return false;
        tf -> notes      = k;
        return tf -> save();
    }
    
    Tag                 tag(const String& k)
    {
        static thread_local SQ    s("SELECT id FROM Tags WHERE k=?");
        return s.as<Tag>(k);
    }
    
    Tag                 tag(uint64_t i)
    {
        return exists_tag(i) ? Tag{i} : Tag{};
    }
    
    TagFile::Shared      tag_doc(Fragment f, bool fAllowEmpty)
    {
        if(!f)
            return TagFile::Shared();

        Vector<char>    ch   = chars(f);
        if(ch.empty())
            return fAllowEmpty ? std::make_shared<TagFile>() : TagFile::Shared();
        
        TagFile::Shared  td = std::make_shared<TagFile>();
        std::filesystem::path   fp  = path(f);
        if(!td->load(ch, fp)){
            yError() << "Unable to read " << fp;
            return TagFile::Shared();
        }
        td -> set_file(fp);
        return td;
    }

    
    TagFile::Shared         write(Tag t, const Root* rt)
    {
        Document    d   = document(t);
        if(!d)
            return TagFile::Shared();
        if(rt && !rt->is_writable(DataRole::Tags))
            return TagFile::Shared();
        Fragment    f   = rt ? fragment(d, rt) : writable(d, DataRole::Tags);
        if(f)
            return tag_doc(f, true);

        Folder      fo  = folder(d);
        if((fo != cdb::top_folder()) && !exists(fo, rt))
            rt -> make_path(key(fo));

        TagFile::Shared ptr  = std::make_shared<TagFile>();
        ptr -> set_file( rt -> resolve(key(d)));
        ptr -> reload();
        return ptr;
    }    
}

