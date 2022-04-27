////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "cdb.hpp"
#include <db/tag/struct.hpp>

namespace yq {

    bool Field::less_key(Field a, Field b)
    {
        return is_less_igCase(cdb::key(a), cdb::key(b));
    }

    bool Field::less_label(Field a, Field b)
    {
        return is_less_igCase(cdb::label(a), cdb::label(b));
    }

    bool Field::less_name(Field a, Field b)
    {
        return is_less_igCase(cdb::name(a), cdb::name(b));
    }


    namespace cdb {
        
        namespace {
            inline std::string field_filename(std::string_view k)
            {
                return make_filename(k, Field::szExtension);
            }
        }

        namespace {
            std::vector<Field>    all_fields_sorted()
            {
                static thread_local SQ s("SELECT id FROM Fields ORDER BY k");
                return s.vec<Field>();
            }
            
            std::vector<Field>    all_fields_unsorted()
            {
                static thread_local SQ s("SELECT id FROM Fields");
                return s.vec<Field>();
            }
        }
        
        std::vector<Field>           all_fields(Sorted sorted)
        {
            return sorted ? all_fields_sorted() : all_fields_unsorted();
        }
        
        size_t              all_fields_count()
        {
            static thread_local SQ s("SELECT COUNT(1) FROM Fields");
            return s.size();
        }
        
        
        std::string             brief(Field f)
        {
            static thread_local SQ s("SELECT brief FROM Fields WHERE id=?");
            return s.str(f.id);
        }

        Category  category(Field f)
        {
            static thread_local SQ s("SELECT category FROM Fields WHERE id=?");
            return s.as<Category>(f.id);
        }
        
        Class               class_(Field f)
        {
            static thread_local SQ s("SELECT class FROM Fields WHERE id=?");
            return s.as<Class>(f.id);
        }

        
        namespace {
            std::vector<Class>       classes_sorted(Field f)
            {
                static thread_local SQ s("SELECT class FROM CFields INNER JOIN Classes ON CFields.class=Classes.id WHERE field=? ORDER BY Classes.k");
                return s.vec<Class>(f.id);
            }

            std::vector<Class>       classes_unsorted(Field f)
            {
                static thread_local SQ s("SELECT class FROM CFields WHERE field=?");
                return s.vec<Class>(f.id);
            }
        }
        
        std::vector<Class>       classes(Field f, Sorted sorted)
        {
            return sorted ? classes_sorted(f) : classes_unsorted(f);
        }

        string_set_t        data_types(Field f)
        {
            static thread_local SQ    s("SELECT type FROM FDataTypes WHERE field=?");
            return s.sset(f.id);
        }

        Field               db_field(Document doc, bool *wasCreated)
        {
            if(wasCreated)
                *wasCreated = false;
            if(!doc)
                return Field{};
            
            if(exists_field(doc.id))
                return Field{doc.id};
            
            std::string k   = skeyc(doc);
            if(k.empty())
                return Field();

            std::string_view    ck  = k;
            Class               cls;
            size_t  n   = ck.find('.');
            if(n != std::string_view::npos){
                cls = class_(ck.substr(0,n));
                ck  = ck.substr(n+1);
            }

            static thread_local SQ i("INSERT INTO Fields (id, k, class, ck) VALUES (?,?,?,?)");
            auto i_af = i.af();
            i.bind(1, doc.id);
            i.bind(2, k);
            i.bind(3, cls.id);
            i.bind(4, ck);
            if(i.exec()){
                if(wasCreated)
                    *wasCreated = true;
                return Field{doc.id};
            } else if(exists_field(doc.id)){
                return Field{doc.id};
            } else {
                yError() << "Unable to get the field from the database: " << k;
                return Field{};
            }
        }
        
        
        namespace {
            std::vector<Class>           def_classes_sorted(Field f)
            {
                static thread_local SQ s("SELECT class FROM FDefClass INNER JOIN Classes ON FDefClass.class=Classes.id WHERE field=? ORDER BY Classes.K");
                return s.vec<Class>(f.id);
            }

            std::vector<Class>           def_classes_unsorted(Field f)
            {
                static thread_local SQ s("SELECT class FROM FDefClass WHERE field=?");
                return s.vec<Class>(f.id);
            }
        }
        
        std::vector<Class>           def_classes(Field f, Sorted sorted)
        {
            return sorted ? def_classes_sorted(f) : def_classes_unsorted(f);
        }

        Document                document(Field f)
        {
            return Document(f.id);
        }

        
        bool                exists(Field f)
        {
            return exists_field(f.id);
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
        
        Field                   field(std::string_view k)
        {
            static thread_local SQ s("SELECT 1 FROM Fields WHERE k=?");
            return s.as<Field>(k);
        }

        Field::SharedFile        field_doc(Fragment f, unsigned int opts)
        {
            if(!f)
                return Field::SharedFile();
                
            std::filesystem::path       fp  = path(f);
            Fragment::Lock  lk;
            if(!(opts & DONT_LOCK)){
                lk  = Fragment::Lock::read(f);
                if(!lk){
                    yWarning() << "Unable to get read lock on fragment: " << fp;
                    return Field::SharedFile();
                }
            }

            auto ch   = file_bytes(fp);
            lk.free();
            if(ch.empty()){
                if(opts & ALLOW_EMPTY)
                    return std::make_shared<Field::File>();
                return Field::SharedFile();
            }
            
                            
            Field::SharedFile  td = std::make_shared<Field::File>();
            if(!td->load(std::move(ch), fp)){
                yError() << "Unable to read " << fp;
                return Field::SharedFile();
            }
            td -> set_file(fp);
            return td;
        }

        Image               icon(Field f)
        {
            static thread_local SQ    s("SELECT icon FROM Fields WHERE id=?");
            return s.as<Image>(f.id);
        }
        
        
        Field::Info         info(Field f, bool autoKey)
        {
            Field::Info        ret;
            static thread_local SQ s("SELECT k, class, name, pkey, plural, brief, category FROM Fields WHERE id=?");
            auto s_af = s.af();
            s.bind(1, f.id);
            if(s.step() == SqlQuery::Row){
                ret.key     = s.v_string(1);
                ret.class_  = Class(s.v_uint64(2));
                ret.name    = s.v_string(3);
                ret.pkey    = s.v_string(4);
                ret.plural  = s.v_string(5);
                ret.brief   = s.v_string(6);
                if(autoKey && ret.name.empty())
                    ret.name    = ret.key;
                ret.category = Category{ s.v_uint64(7)};
            }
            return ret;
        }
        
        std::string             key(Field f)
        {
            static thread_local SQ s("SELECT k FROM Fields WHERE id=?");
            return s.str(f.id);
        }
        
        std::string             label(Field f)
        {
            static thread_local SQ    s("SELECT coalesce(name,ck,k) FROM Fields WHERE id=?");
            return s.str(f.id);
        }

        Field::SharedData            merged(Field f, unsigned int opts)
        {
            Field::SharedData        ret = std::make_shared<Field::Data>();;
            for(auto& i : reads(f, opts)){
                if(opts & IS_UPDATE)
                    update(i.first);
                ret->merge(*(i.second), static_cast<bool>(opts&OVERRIDE));
            }
            return ret;
        }
        


        std::string             name(Field f)
        {
            static thread_local SQ    s("SELECT name FROM Fields WHERE id=?");
            return s.str(f.id);
        }

        NKI                     nki(Field f, bool autoKeyToName)
        {
            static thread_local SQ    s("SELECT name,icon,k FROM Fields WHERE id=?");
            auto s_af = s.af();
            s.bind(1, f.id);
            if(s.step() == SqlQuery::Row){
                NKI  ret;
                ret.name    = s.v_string(1);
                ret.icon    = Image(s.v_uint64(2)) ;
                ret.key     = s.v_string(3);
                if(autoKeyToName && ret.name.empty())
                    ret.name    = ret.key;
                return ret;
            }
            return NKI{};
        }    
        
        
        Class               parent(Field f)
        {
            return class_(f);
        }

        
        std::string             pkey(Field f)
        {
            static thread_local SQ s("SELECT pkey FROM Fields WHERE id=?");
            return s.str(f.id);
        }
        
        std::string             plural(Field f)
        {
            static thread_local SQ s("SELECT plural FROM Fields WHERE id=?");
            return s.str(f.id);
        }
        

        Field::SharedFile        read(Field f, const Root*rt, unsigned int opts)
        {
            return field_doc(fragment(document(f), rt), opts);
        }

        std::vector<FieldFragDoc>    reads(Field f, unsigned int opts)
        {
            std::vector<FieldFragDoc>  ret;
            for(Fragment ff : fragments(document(f), DataRole::Config)){
                Field::SharedFile    p   = field_doc(ff, opts);
                if(p)
                    ret.push_back(FieldFragDoc(ff, p));
            }
            return ret;
        }

        std::vector<FieldFragDoc>  reads(Field f, class Root*rt, unsigned int opts)
        {
            std::vector<FieldFragDoc>  ret;
            for(Fragment ff : fragments(document(f), rt)){
                Field::SharedFile    p   = field_doc(ff, opts);
                if(p)
                    ret.push_back(FieldFragDoc(ff, p));
            }
            return ret;
        }

        namespace {
            std::vector<Tag>  tags_sorted(Field f)
            {
                static thread_local SQ s("SELECT tag FROM FTags INNER JOIN Tags ON FTags.tag=Tags.id WHERE field=? ORDER BY Tags.K");
                return s.vec<Tag>(f.id);
            }
            
            std::vector<Tag>  tags_unsorted(Field f)
            {
                static thread_local SQ s("SELECT tag FROM FTags WHERE field=?");
                return s.vec<Tag>(f.id);
            }
        }
        
        std::vector<Tag>  tags(Field f, Sorted sorted)
        {
            return sorted ? tags_sorted(f) : tags_unsorted(f);
        }

        
        size_t  tags_count(Field f)
        {
            static thread_local SQ s("SELECT COUNT(1) FROM FTags WHERE field=?");
            return s.size(f.id);
        }

        
        Field::SharedFile        writable(Field f, const Root* rt, unsigned int opts)
        {
            Document    d   = document(f);
            if(!d)
                return Field::SharedFile();
            if(rt && !rt->is_writable(DataRole::Config))
                return Field::SharedFile();

            Fragment    ff   = fragment(d);
            if(ff)
                return field_doc(ff, opts | ALLOW_EMPTY);
                
            Folder      fo  = folder(d);
            if((fo != cdb::top_folder()) && !exists(fo, rt))
                make_directory(fo, rt);
            Field::SharedFile ptr  = std::make_shared<Field::File>();
            ptr -> set_file( rt -> resolve(key(d)));
            return ptr;
        }
    }
}
