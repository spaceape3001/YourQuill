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
#include <kernel/agw/ClassCDB.hpp>
#include <kernel/agw/FieldFile.hpp>
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
                return make_filename(k, agw::Field::szExtension);
            }
        }

        string_set_t            aliases(agw::Field f)
        {
            static thread_local SQ s("SELECT alias FROM FAlias WHERE field=?");
            return s.sset(f.id);
        }
        
        agw::FieldVector       all_fields(Sorted sorted)
        {
            static thread_local SQ qs("SELECT id FROM Fields ORDER BY k");
            static thread_local SQ qu("SELECT id FROM Fields");
            SQ& s = sorted ? qs : qu;
            return s.vec<agw::Field>();
        }
        
        size_t              all_fields_count()
        {
            static thread_local SQ s("SELECT COUNT(1) FROM Fields");
            return s.size();
        }
        
        
        std::string             brief(agw::Field f)
        {
            static thread_local SQ s("SELECT brief FROM Fields WHERE id=?");
            return s.str(f.id);
        }

        Category  category(agw::Field f)
        {
            static thread_local SQ s("SELECT category FROM Fields WHERE id=?");
            return s.as<Category>(f.id);
        }
        
        agw::Class               class_(agw::Field f)
        {
            static thread_local SQ s("SELECT class FROM Fields WHERE id=?");
            return s.as<agw::Class>(f.id);
        }

        
        agw::ClassVector       classes(agw::Field f, Sorted sorted)
        {
            static thread_local SQ qs("SELECT class FROM CFields INNER JOIN agw::Classes ON CFields.class=agw::Classes.id WHERE field=? ORDER BY agw::Classes.k");
            static thread_local SQ qu("SELECT class FROM CFields WHERE field=?");
            SQ& s = sorted ? qs : qu;
            return s.vec<agw::Class>(f.id);
        }

        std::set<uint64_t>        data_types(agw::Field f)
        {
            static thread_local SQ    s("SELECT type FROM FDataTypes WHERE field=?");
            return s.set<uint64_t>(f.id);
        }

        agw::Field               db_field(Document doc, bool *wasCreated)
        {
            if(wasCreated)
                *wasCreated = false;
            if(!doc)
                return agw::Field{};
            
            if(exists_field(doc.id))
                return agw::Field{doc.id};
            
            std::string k   = skeyc(doc);
            if(k.empty())
                return agw::Field();

            std::string_view    ck  = k;
            agw::Class               cls;
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
                return agw::Field{doc.id};
            } else if(exists_field(doc.id)){
                return agw::Field{doc.id};
            } else {
                yError() << "Unable to get the field from the database: " << k;
                return agw::Field{};
            }
        }
        
        agw::ClassVector           def_classes(agw::Field f, Sorted sorted)
        {
            static thread_local SQ qs("SELECT class FROM CFields INNER JOIN agw::Classes ON FDefagw::Class.class=agw::Classes.id WHERE field=? ORDER BY agw::Classes.K");
            static thread_local SQ qu("SELECT class FROM CFields WHERE field=?");
            SQ& s = sorted ? qs : qu;
            return s.vec<agw::Class>(f.id);
        }

        Document                document(agw::Field f)
        {
            return Document(f.id);
        }
        
        
        bool                exists(agw::Field f)
        {
            return exists_field(f.id);
        }
        

        bool                exists_field(uint64_t i)
        {
            static thread_local SQ s("SELECT 1 FROM Fields WHERE id=?");
            return s.present(i);
        }

        agw::Field               field(uint64_t i)
        {
            return exists_field(i) ? agw::Field{i} : agw::Field{};
        }
        
        agw::Field                   field(Document doc, bool calc)
        {
            if(!doc)
                return agw::Field();
            if(exists_field(doc.id))
                return agw::Field{doc.id};
            if(calc && (folder(doc) == fields_folder())){
                std::string k   = skeyc(doc);
                return field(k);
            }
            return agw::Field();
        }

        agw::Field                   field(std::string_view k)
        {
            static thread_local SQ s("SELECT 1 FROM Fields WHERE k=?");
            return s.as<agw::Field>(k);
        }

        agw::Field::SharedFile        field_doc(Fragment f, cdb_options_t opts)
        {
            if(!f)
                return agw::Field::SharedFile();
                
            std::filesystem::path       fp  = path(f);
            Fragment::Lock  lk;
            if(!(opts & DONT_LOCK)){
                lk  = Fragment::Lock::read(f);
                if(!lk){
                    yWarning() << "Unable to get read lock on fragment: " << fp;
                    return agw::Field::SharedFile();
                }
            }

            auto ch   = file_bytes(fp);
            lk.free();
            if(ch.empty()){
                if(opts & ALLOW_EMPTY)
                    return std::make_shared<agw::Field::File>();
                return agw::Field::SharedFile();
            }
            
                            
            agw::Field::SharedFile  td = std::make_shared<agw::Field::File>();
            if(!td->load(std::move(ch), fp)){
                yError() << "Unable to read " << fp;
                return agw::Field::SharedFile();
            }
            td -> set_file(fp);
            return td;
        }

        Image               icon(agw::Field f)
        {
            static thread_local SQ    s("SELECT icon FROM Fields WHERE id=?");
            return s.as<Image>(f.id);
        }
        
        
        agw::Field::Info         info(agw::Field f, bool autoKey)
        {
            agw::Field::Info        ret;
            static thread_local SQ s("SELECT k, class, name, pkey, plural, brief, category FROM Fields WHERE id=?");
            auto s_af = s.af();
            s.bind(1, f.id);
            if(s.step() == SqlQuery::Row){
                ret.key     = s.v_string(1);
                ret.class_  = agw::Class(s.v_uint64(2));
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
        
        std::string             key(agw::Field f)
        {
            static thread_local SQ s("SELECT k FROM Fields WHERE id=?");
            return s.str(f.id);
        }
        
        std::string             label(agw::Field f)
        {
            std::string s = name(f);
            if(s.empty())
                s       = key(f);
            return s;

            //static thread_local SQ    s("SELECT coalesce(name,ck,k) FROM Fields WHERE id=?");
            //return s.str(f.id);
        }

        agw::Field                       make_field(std::string_view kf, agw::Class c, const Root* rt, cdb_options_t opts, bool *wasCreated)
        {
        
            if(wasCreated)
                *wasCreated = false;
            if(kf.empty()){
                yError() << "Cannot create a BLANK field!";
                return agw::Field();
            }
            if(!rt)
                rt      = wksp::root_first(DataRole::Config);
            if(!rt){
                yError() << "No root specified to create the class in!";
                return agw::Field{};
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
            agw::Field           f   = db_field(doc, &was);
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
            agw::Field::Lock   lk;
            if(!(opts & DONT_LOCK))
                lk  = agw::Field::Lock::write(f);

            agw::Field::SharedFile    td  = writable(f, rt, DONT_LOCK);
            td -> name      = kf;
            td -> plural    = td->name + 's';
            td -> save();
            return f;
        }

        agw::Field::SharedData            merged(agw::Field f, cdb_options_t opts)
        {
            agw::Field::SharedData        ret = std::make_shared<agw::Field::Data>();;
            for(auto& i : reads(f, opts)){
                if(opts & IS_UPDATE)
                    update(i.first);
                ret->merge(*(i.second), static_cast<bool>(opts&OVERRIDE));
            }
            return ret;
        }
        


        std::string             name(agw::Field f)
        {
            static thread_local SQ    s("SELECT name FROM Fields WHERE id=?");
            return s.str(f.id);
        }

        NKI                     nki(agw::Field f, bool autoKeyToName)
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
        
        
        agw::Class               parent(agw::Field f)
        {
            return class_(f);
        }

        
        std::string             pkey(agw::Field f)
        {
            static thread_local SQ s("SELECT pkey FROM Fields WHERE id=?");
            return s.str(f.id);
        }
        
        std::string             plural(agw::Field f)
        {
            static thread_local SQ s("SELECT plural FROM Fields WHERE id=?");
            return s.str(f.id);
        }
        

        agw::Field::SharedFile        read(agw::Field f, const Root*rt, cdb_options_t opts)
        {
            return field_doc(fragment(document(f), rt), opts);
        }

        std::vector<agw::FieldFragDoc>    reads(agw::Field f, cdb_options_t opts)
        {
            std::vector<agw::FieldFragDoc>  ret;
            for(Fragment ff : fragments(document(f), DataRole::Config)){
                agw::Field::SharedFile    p   = field_doc(ff, opts);
                if(p)
                    ret.push_back(agw::FieldFragDoc(ff, p));
            }
            return ret;
        }

        std::vector<agw::FieldFragDoc>  reads(agw::Field f, class Root*rt, cdb_options_t opts)
        {
            std::vector<agw::FieldFragDoc>  ret;
            for(Fragment ff : fragments(document(f), rt)){
                agw::Field::SharedFile    p   = field_doc(ff, opts);
                if(p)
                    ret.push_back(agw::FieldFragDoc(ff, p));
            }
            return ret;
        }

        std::vector<Tag>  tags(agw::Field f, Sorted sorted)
        {
            static thread_local SQ qs("SELECT tag FROM FTags INNER JOIN Tags ON FTags.tag=Tags.id WHERE field=? ORDER BY Tags.K");
            static thread_local SQ qu("SELECT tag FROM FTags WHERE field=?");
            SQ& s = sorted ? qs : qu;
            return s.vec<Tag>(f.id);
        }

        
        size_t  tags_count(agw::Field f)
        {
            static thread_local SQ s("SELECT COUNT(1) FROM FTags WHERE field=?");
            return s.size(f.id);
        }

        std::set<Tag>            tags_set(agw::Field f)
        {
            static thread_local SQ s("SELECT tag FROM FTags WHERE field=?");
            return s.set<Tag>(f.id);
        }

        agw::Field::SharedFile        writable(agw::Field f, const Root* rt, cdb_options_t opts)
        {
            if(!f)
                return agw::Field::SharedFile();
        
            Document    d   = document(f);
            if(!d){
                yWarning() << "write(agw::Field '" << key(f) << "'): Has no document!";
                return agw::Field::SharedFile();
            }
            if(rt && !rt->is_writable(DataRole::Config)){
                yWarning() << "write(agw::Field '" << key(f) << "'): Root " << rt->key << " cannot be written to!";
                return agw::Field::SharedFile();
            }

            Fragment    ff   = fragment(d);
            if(ff)
                return field_doc(ff, opts | ALLOW_EMPTY);
                
            Folder      fo  = folder(d);
            if((fo != cdb::top_folder()) && !exists(fo, rt))
                make_directory(fo, rt);
            agw::Field::SharedFile ptr  = std::make_shared<agw::Field::File>();
            ptr -> set_file( rt -> resolve(key(d)));
            return ptr;
        }
    }
}
