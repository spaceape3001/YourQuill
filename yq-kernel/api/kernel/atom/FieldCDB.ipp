////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FieldCDB.hpp"
#include <basic/ByteArray.hpp>
#include <basic/CollectionUtils.hpp>
#include <basic/FileUtils.hpp>
#include <basic/meta/TypeInfo.hpp>
#include <kernel/atom/ClassCDB.hpp>
#include <kernel/atom/FieldFile.hpp>
#include <kernel/db/IDLock.hpp>
#include <kernel/db/NKI.hpp>
#include <kernel/db/SQ.hpp>
#include <kernel/file/DocumentCDB.hpp>
#include <kernel/file/FolderCDB.hpp>
#include <kernel/file/FragmentCDB.hpp>
#include <kernel/file/Root.hpp>
#include <kernel/image/ImageCDB.hpp>
#include <kernel/org/CategoryCDB.hpp>
#include <kernel/org/TagCDB.hpp>
#include <kernel/wksp/Workspace.hpp>

namespace yq {

    namespace cdb {
        namespace {
            inline std::string field_filename(std::string_view k)
            {
                return make_filename(k, Field::szExtension);
            }
        }

        string_set_t            aliases(Field f)
        {
            static thread_local SQ s("SELECT alias FROM FAlias WHERE field=?");
            return s.sset(f.id);
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

        std::set<uint64_t>        data_types(Field f)
        {
            static thread_local SQ    s("SELECT type FROM FDataTypes WHERE field=?");
            return s.set<uint64_t>(f.id);
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
                static thread_local SQ s("SELECT class FROM CFields INNER JOIN Classes ON FDefClass.class=Classes.id WHERE field=? ORDER BY Classes.K");
                return s.vec<Class>(f.id);
            }

            std::vector<Class>           def_classes_unsorted(Field f)
            {
                static thread_local SQ s("SELECT class FROM CFields WHERE field=?");
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
        
        void                erase(Field x)
        {
            static thread_local SQ stmts[] = {
                SQ("DELETE FROM CFields WHERE field=?"),
                SQ("DELETE FROM FTags WHERE field=?"),
                SQ("DELETE FROM Fields WHERE id=?")
            };

            if(!x)
                return ;
            
            for(auto& sq : stmts)
                sq.exec(x.id);
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
            static thread_local SQ s("SELECT 1 FROM Fields WHERE k=?");
            return s.as<Field>(k);
        }

        Field::SharedFile        field_doc(Fragment f, cdb_options_t opts)
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
            std::string s = name(f);
            if(s.empty())
                s       = key(f);
            return s;

            //static thread_local SQ    s("SELECT coalesce(name,ck,k) FROM Fields WHERE id=?");
            //return s.str(f.id);
        }

        Field                       make_field(std::string_view kf, Class c, const Root* rt, cdb_options_t opts, bool *wasCreated)
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
                yError() << "No root specified to create the class in!";
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
            Field::Lock   lk;
            if(!(opts & DONT_LOCK))
                lk  = Field::Lock::write(f);

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
        

        Field::SharedFile        read(Field f, const Root*rt, cdb_options_t opts)
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

        std::vector<FieldFragDoc>  reads(Field f, class Root*rt, cdb_options_t opts)
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

        std::set<Tag>            tags_set(Field f)
        {
                static thread_local SQ s("SELECT tag FROM FTags WHERE field=?");
                return s.set<Tag>(f.id);
        }

        Field::SharedData       update(Field x, cdb_options_t opts)
        {
            if(!x)
                return Field::SharedData();
                
            if(opts & U_ICON)
                update_icon(x);

            Field::SharedData data = merged(x, IS_UPDATE | opts);
            if(!data)
                return Field::SharedData();
            
            static thread_local SQ iAlias("INSERT INTO FAlias (field, alias) VALUES (?,?)");
            static thread_local SQ dAlias("DELETE FROM FAlias WHERE field=? AND alias=?");
            static thread_local SQ iTypes("INSERT INTO FDataTypes (field, type) VALUES (?,?)");
            static thread_local SQ dTypes("DELETE FROM FDataTypes WHERE field=? AND type=?");
            static thread_local SQ iTags("INSERT INTO FTags (field, tag) VALUES (?, ?)");
            static thread_local SQ dTags("DELETE FROM FTags WHERE field=? AND tag=?");
            static thread_local SQ uInfo("UPDATE Fields SET name=?,brief=?,multi=?,restrict=?,category=?,pkey=?,expected=?,maxcnt=?,plural=? WHERE id=?");
            
            if(opts & U_INFO){
                Category cat = category(data->category);

                uInfo.bind(1, data->name);
                uInfo.bind(2, data->brief);
                uInfo.bind(3, data->multiplicity.value());
                uInfo.bind(4, data->restriction.value());
                uInfo.bind(5, cat.id);
                uInfo.bind(6, data->pkey);
                uInfo.bind(7, data->expected);
                uInfo.bind(8, data->max_count);
                uInfo.bind(9, data->plural);
                uInfo.bind(10, x.id);
                uInfo.exec();
                
                
                string_set_t    old_aliases = aliases(x);
                std::set<uint64_t> old_types = data_types(x);
                std::set<uint64_t> new_types = ids_for(TypeInfo::find_all(data->types, true));
                
                auto ch_aliases = add_remove(old_aliases, data->aliases);
                auto ch_types   = add_remove(old_types, new_types);
                
                iAlias.batch(x.id, ch_aliases.added);
                dAlias.batch(x.id, ch_aliases.removed);
                iTypes.batch(x.id, ch_types.added);
                dTypes.batch(x.id, ch_types.removed);
            }
            
            if(opts & U_TAGS){
                std::set<Tag>       old_tags    = tags_set(x);
                std::set<Tag>       new_tags    = tags_set(data->tags, true);
                
                auto ch_tags    = add_remove(old_tags, new_tags);
                iTags.batch(x.id, ids_for(ch_tags.added));
                dTags.batch(x.id, ids_for(ch_tags.removed));
            }
            
            //  AND MORE TO GO.....
            
            return data;
        }
        
        void                update_icon(Field x)
        {
            if(!x)
                return ;
                
            Document    doc     = document(x);
            Image       img     = best_image(doc);
            
            static thread_local SQ u1("UPDATE Fields SET icon=? WHERE id=?");
            static thread_local SQ u2("UPDATE Documents SET icon=? WHERE id=?");

            if(icon(doc) != img){
                u1.exec(img.id, x.id);
                u2.exec(doc.id, x.id);
            }
        }

        
        Field::SharedFile        writable(Field f, const Root* rt, cdb_options_t opts)
        {
            if(!f)
                return Field::SharedFile();
        
            Document    d   = document(f);
            if(!d){
                yWarning() << "write(Field '" << key(f) << "'): Has no document!";
                return Field::SharedFile();
            }
            if(rt && !rt->is_writable(DataRole::Config)){
                yWarning() << "write(Field '" << key(f) << "'): Root " << rt->key << " cannot be written to!";
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
}
