////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FieldCDB.hpp"
#include <0/basic/ByteArray.hpp>
#include <0/basic/CollectionUtils.hpp>
#include <0/io/FileUtils.hpp>
#include <0/meta/TypeInfo.hpp>
#include <mithril/class/ClassCDB.hpp>
#include <mithril/field/FieldFile.hpp>
#include <mithril/bit/NKI.hpp>
#include <mithril/document/DocumentCDB.hpp>
#include <mithril/folder/FolderCDB.hpp>
#include <mithril/fragment/FragmentCDB.hpp>
#include <mithril/root/RootDir.hpp>
#include <mithril/image/ImageCDB.hpp>
#include <mithril/category/CategoryCDB.hpp>
#include <mithril/tag/TagCDB.hpp>
#include <mithril/wksp/CacheQuery.hpp>
#include <mithril/wksp/Workspace.hpp>

namespace yq::mithril::cdb {
    namespace {
        inline std::string field_filename(std::string_view k)
        {
            return make_filename(k, Field::EXTENSION);
        }
    }

    string_set_t            aliases(Field f)
    {
        static thread_local CacheQuery s("SELECT alias FROM FAlias WHERE field=?");
        return s.sset(f.id);
    }
    
    std::vector<Field>           all_fields(Sorted sorted)
    {
        static thread_local CacheQuery qs("SELECT id FROM Fields ORDER BY k");
        static thread_local CacheQuery qu("SELECT id FROM Fields");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Field>();
    }
    
    size_t              all_fields_count()
    {
        static thread_local CacheQuery s("SELECT COUNT(1) FROM Fields");
        return s.size();
    }
    
    //ClassHopMap             atom_types(Field f)
    //{
        //ClassHopMap ret;
        //static thread_local CacheQuery s("SELECT class, hops FROM CFields WHERE field=?");
        //auto af = s.af();
        //s.bind(1, f.id);
        //while(s.step() == SQResult::Row)
            //ret[Class(s.v_uint64(1))] = { (int8_t) s.v_int(2) };
        //return ret;
    //}
    
    std::string             brief(Field f)
    {
        static thread_local CacheQuery s("SELECT brief FROM Fields WHERE id=?");
        return s.str(f.id);
    }

    Category  category(Field f)
    {
        static thread_local CacheQuery s("SELECT category FROM Fields WHERE id=?");
        return s.as<Category>(f.id);
    }
    
    Class               class_(Field f)
    {
        static thread_local CacheQuery s("SELECT class FROM Fields WHERE id=?");
        return s.as<Class>(f.id);
    }

    
    std::vector<Class>       classes(Field f, Sorted sorted)
    {
        static thread_local CacheQuery qs("SELECT class FROM CFields INNER JOIN Classes ON CFields.class=Classes.id WHERE field=? ORDER BY Classes.k");
        static thread_local CacheQuery qu("SELECT class FROM CFields WHERE field=?");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Class>(f.id);
    }

    //std::set<uint64_t>        data_types(Field f)
    //{
        //static thread_local CacheQuery    s("SELECT type FROM FDataTypes WHERE field=?");
        //return s.set<uint64_t>(f.id);
    //}

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
        bool    anycls  = true;
        if(n != std::string_view::npos){
            cls = class_(ck.substr(0,n));
            ck  = ck.substr(n+1);
            anycls  = false;
        }

        static thread_local CacheQuery i("INSERT INTO Fields (id, k, class, ck, anycls) VALUES (?,?,?,?,?)");
        auto i_af = i.af();
        i.bind(1, doc.id);
        i.bind(2, k);
        i.bind(3, cls.id);
        i.bind(4, ck);
        i.bind(5, anycls);
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

    std::vector<Class>           def_classes(Field f, Sorted sorted)
    {
        static thread_local CacheQuery qs("SELECT class FROM CFields INNER JOIN Classes ON FDefClass.class=Classes.id WHERE field=? ORDER BY Classes.K");
        static thread_local CacheQuery qu("SELECT class FROM CFields WHERE field=?");
        auto& q = sorted ? qs : qu;
        return q.vec<Class>(f.id);
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
        static thread_local CacheQuery s("SELECT 1 FROM Fields WHERE id=?");
        return s.present(i);
    }

    Id                  expected(Field f)
    {
        static thread_local CacheQuery    s("SELECT expected FROM Fields WHERE id=?");
        return Id{s.u64(f.id)};
    }

    Field               field(uint64_t i)
    {
        return exists_field(i) ? Field{i} : Field{};
    }
    
    Field                   field(Document doc, bool calc)
    {
        if(!doc)
            return Field();
        if(exists_field(doc.id))
            return Field{doc.id};
        if(calc && (folder(doc) == fields_folder())){
            std::string k   = skeyc(doc);
            return field(k);
        }
        return Field();
    }

    Field                   field(std::string_view k)
    {
        static thread_local CacheQuery s("SELECT 1 FROM Fields WHERE k=?");
        return s.as<Field>(k);
    }

    Field::SharedFile        field_doc(Fragment f, cdb_options_t opts)
    {
        if(!f)
            return Field::SharedFile();
            
        std::filesystem::path       fp  = path(f);
        Id::Lock  lk;
        if(!(opts & DONT_LOCK)){
            lk  = Id(f).lock(false);
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
        if(td->load(std::move(ch), fp) != std::error_code()){
            yError() << "Unable to read " << fp;
            return Field::SharedFile();
        }
        td -> set_file(fp);
        return td;
    }

    Image               icon(Field f)
    {
        static thread_local CacheQuery    s("SELECT icon FROM Fields WHERE id=?");
        return s.as<Image>(f.id);
    }
    
    
    Field::Info         info(Field f, bool autoKey)
    {
        Field::Info        ret;
        static thread_local CacheQuery s("SELECT k, class, name, pkey, plural, brief, category, restrict, multi, maxcnt, expected FROM Fields WHERE id=?");
        auto s_af = s.af();
        s.bind(1, f.id);
        if(s.step() == SQResult::Row){
            ret.key             = s.v_string(1);
            ret.class_          = Class(s.v_uint64(2));
            ret.name            = s.v_string(3);
            ret.pkey            = s.v_string(4);
            ret.plural          = s.v_string(5);
            ret.brief           = s.v_string(6);
            ret.category        = Category{ s.v_uint64(7)};
            ret.restriction     = (Restriction) s.v_int(8);
            ret.multiplicity    = (Multiplicity) s.v_int(9);
            ret.max_count       = s.v_uint64(10);
            ret.expected        = Id{ s.v_uint64(11) };
            if(autoKey && ret.name.empty())
                ret.name        = ret.key;
        }
        return ret;
    }
    
    bool                    is_any(Field f)
    {
        static thread_local CacheQuery s("SELECT anycls FROM Fields WHERE id=?");
        return s.boolean(f.id);
    }

    std::string             key(Field f)
    {
        static thread_local CacheQuery s("SELECT k FROM Fields WHERE id=?");
        return s.str(f.id);
    }
    
    std::string             label(Field f)
    {
        std::string s = name(f);
        if(s.empty())
            s       = key(f);
        return s;

        //static thread_local CacheQuery    s("SELECT coalesce(name,ck,k) FROM Fields WHERE id=?");
        //return s.str(f.id);
    }

    Field                       make_field(std::string_view kf, Class c, const RootDir* rt, cdb_options_t opts, bool *wasCreated)
    {
    
        if(wasCreated)
            *wasCreated = false;
        if(kf.empty()){
            yError() << "Cannot create a BLANK field!";
            return Field();
        }
        if(!rt)
            rt      = wksp::root_first(DataRole::Config);
        if(!rt){
            yError() << "No root_dir specified to create the class in!";
            return Field{};
        }
        
        std::string         k;
        if(c){
            k   = key(c) + "." + std::string(kf);
        } else {
            k   = kf;
        }
        
        std::string cfn = field_filename(k);
        Document        doc = db_document(fields_folder(), cfn);
        bool            was = false;
        Field           f   = db_field(doc, &was);
        if(!f){
            yWarning() << "Unable to create/find class: " << k;
            return f;
        }
        if(wasCreated)
            *wasCreated = was;
        if(!was)
            return f;
            
        if(fragments_count(doc))
            return f;
        Id::Lock   lk;
        if(!(opts & DONT_LOCK))
            lk  = Id(f).lock(true);

        Field::SharedFile    td  = writable(f, rt, DONT_LOCK);
        td -> name      = kf;
        td -> plural    = td->name + 's';
        td -> save();
        return f;
    }

    Field::SharedData            merged(Field f, cdb_options_t opts)
    {
        Field::SharedData        ret = std::make_shared<Field::Data>();;
        for(auto& i : reads(f, opts)){
            if(opts & IS_UPDATE)
                update(i.first);
            ret->merge(*(i.second), static_cast<bool>(opts&OVERRIDE));
        }
        return ret;
    }

    uint64_t                max_count(Field f)
    {
        static thread_local CacheQuery    s("SELECT maxcnt FROM Fields WHERE id=?");
        return s.u64(f);
    }
    
    Multiplicity            multiplicity(Field f)
    {
        static thread_local CacheQuery    s("SELECT multi FROM Fields WHERE id=?");
        return Multiplicity(s.integer(f));
    }

    std::string             name(Field f)
    {
        static thread_local CacheQuery    s("SELECT name FROM Fields WHERE id=?");
        return s.str(f.id);
    }

    NKI                     nki(Field f, bool autoKeyToName)
    {
        static thread_local CacheQuery    s("SELECT name,icon,k FROM Fields WHERE id=?");
        auto s_af = s.af();
        s.bind(1, f.id);
        if(s.step() == SQResult::Row){
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
        static thread_local CacheQuery s("SELECT pkey FROM Fields WHERE id=?");
        return s.str(f.id);
    }
    
    std::string             plural(Field f)
    {
        static thread_local CacheQuery s("SELECT plural FROM Fields WHERE id=?");
        return s.str(f.id);
    }
    

    Field::SharedFile        read(Field f, const RootDir*rt, cdb_options_t opts)
    {
        return field_doc(fragment(document(f), rt), opts);
    }

    std::vector<FieldFragDoc>    reads(Field f, cdb_options_t opts)
    {
        std::vector<FieldFragDoc>  ret;
        for(Fragment ff : fragments(document(f), DataRole::Config)){
            Field::SharedFile    p   = field_doc(ff, opts);
            if(p)
                ret.push_back(FieldFragDoc(ff, p));
        }
        return ret;
    }

    std::vector<FieldFragDoc>  reads(Field f, class RootDir*rt, cdb_options_t opts)
    {
        std::vector<FieldFragDoc>  ret;
        for(Fragment ff : fragments(document(f), rt)){
            Field::SharedFile    p   = field_doc(ff, opts);
            if(p)
                ret.push_back(FieldFragDoc(ff, p));
        }
        return ret;
    }

    Restriction             restriction(Field f)
    {
        static thread_local CacheQuery    s("SELECT restrict FROM Fields WHERE id=?");
        return Restriction(s.integer(f));
    }

    std::string             skey(Field f)
    {
        static thread_local CacheQuery s("SELECT ck FROM Fields WHERE id=?");
        return s.str(f.id);
    }


    std::vector<Tag>  tags(Field f, Sorted sorted)
    {
        static thread_local CacheQuery qs("SELECT tag FROM FTags INNER JOIN Tags ON FTags.tag=Tags.id WHERE field=? ORDER BY Tags.K");
        static thread_local CacheQuery qu("SELECT tag FROM FTags WHERE field=?");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Tag>(f.id);
    }

    
    size_t  tags_count(Field f)
    {
        static thread_local CacheQuery s("SELECT COUNT(1) FROM FTags WHERE field=?");
        return s.size(f.id);
    }

    std::set<Tag>            tags_set(Field f)
    {
        static thread_local CacheQuery s("SELECT tag FROM FTags WHERE field=?");
        return s.set<Tag>(f.id);
    }

    std::vector<Id>         types(Field f)
    {
        static thread_local CacheQuery s("SELECT type FROM FTypes WHERE field=?");
        return s.vec<Id>(f.id);
    }

    Field::SharedFile        writable(Field f, const RootDir* rt, cdb_options_t opts)
    {
        if(!f)
            return Field::SharedFile();
    
        Document    d   = document(f);
        if(!d){
            yWarning() << "write(Field '" << key(f) << "'): Has no document!";
            return Field::SharedFile();
        }
        if(rt && !rt->is_writable(DataRole::Config)){
            yWarning() << "write(Field '" << key(f) << "'): RootDir " << rt->key << " cannot be written to!";
            return Field::SharedFile();
        }

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
