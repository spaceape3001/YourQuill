////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/orgsys/category-cdb.hpp>
#include <db/orgsys/tag-cdb.hpp>

namespace yq {
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

        namespace {
            inline std::string class_filename(std::string_view k)
            {
                return make_filename(k, Class::szExtension);
            }
        }

        string_set_t         aliases(Class c)
        {
            static thread_local SQ s("SELECT alias FROM CAlias WHERE class=?");
            return s.sset(c.id);
        }

        namespace {
            std::vector<Class>    all_classes_sorted()
            {
                static thread_local SQ    s("SELECT id FROM Classes ORDER BY K");
                return s.vec<Class>();
            }
            
            std::vector<Class>    all_classes_unsorted()
            {
                static thread_local SQ    s("SELECT id FROM Classes");
                return s.vec<Class>();
            }
        }

        std::vector<Class>  all_classes(Sorted sorted)
        {
            return sorted ? all_classes_sorted() : all_classes_unsorted();
        }

        size_t  all_classes_count()
        {
            static thread_local SQ    s("SELECT COUNT(1) FROM Classes");
            return s.size();
        }
        
        
        std::string  binding(Class c)
        {
            static thread_local SQ  s("SELECT binding FROM Classes WHERE id=?");
            return s.str(c.id);
        }

        std::string  brief(Class c)
        {
            static thread_local SQ    s("SELECT brief FROM Classes WHERE id=?");
            return s.str(c.id);
        }
        
        Category  category(Class c)
        {
            static thread_local SQ s("SELECT category FROM Classes WHERE id=?");
            return s.as<Category>(c.id);
        }

        Class  class_(uint64_t i)
        {
            return exists_class(i) ? Class{i} : Class();
        }

        Class  class_(Document doc, bool calc)
        {
            if(!doc)
                return Class();
            if(exists_class(doc.id))
                return Class{doc.id};
            if(calc && (folder(doc) == classes_folder())){
                std::string k   = base_key(doc);
                return class_(k);
            }
            
            return Class();
        }

        
        Class  class_(std::string_view  k)
        {
            static thread_local SQ s("SELECT id FROM Classes WHERE k=?");
            return s.as<Class>(k);
        }

        Class::SharedFile  class_doc(Fragment f, unsigned int opts)
        {
            if(!f)
                return Class::SharedFile();
                
            std::filesystem::path       fp  = path(f);
            Fragment::Lock  lk;
            if(!(opts & DONT_LOCK)){
                lk  = Fragment::Lock::read(f);
                if(!lk){
                    yWarning() << "Unable to get read lock on fragment: " << fp;
                    return Class::SharedFile();
                }
            }

            auto ch   = file_bytes(fp);
            lk.free();
            if(ch.empty()){
                if(opts & ALLOW_EMPTY)
                    return std::make_shared<Class::File>();
                return Class::SharedFile();
            }
                
            Class::SharedFile  td = std::make_shared<Class::File>();
            if(!td->load(std::move(ch), fp)){
                yError() << "Unable to read " << fp;
                return Class::SharedFile();
            }
            td -> set_file(fp);
            return td;
        }
        

        std::vector<Class>       classes(const string_set_t& sset, bool noisy)
        {
            std::vector<Class>   ret;
            for(const std::string& s : sset){
                if(s.empty())
                    continue;
                    
                Class cls = class_(s);
                if(!cls){
                    if(noisy)
                        yWarning() << "Unable to find class: " << s;
                    continue;
                }
                
                ret.push_back(cls);
            }
            return ret;
        }
        
        Class               db_class(std::string_view k, bool *wasCreated)
        {
            std::string     tfn = class_filename(k);
            return db_class(db_document(classes_folder(), tfn), wasCreated);
        }
        
        Class               db_class(Document doc, bool *wasCreated)
        {
            if(wasCreated)
                *wasCreated = false;
            if(folder(doc) != classes_folder())
                return Class{};
            std::string k   = base_key(doc);
            if(k.empty())
                return Class();
        
            static thread_local SQ    i("INSERT OR FAIL INTO Classes (id,k) VALUES (?,?)");
            auto i_lk   = i.af();

            i.bind(1, doc.id);
            i.bind(2, k);
            if(i.exec()){
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

        std::vector<Class>            db_classes(const string_set_t& all)
        {
            std::vector<Class>   ret;
            for(std::string_view  s: all)
                ret.push_back(db_class(s));
            return ret;
        }
        
        namespace {
            std::vector<Class>           def_derived_sorted(Class c)
            {   
                static thread_local SQ s("SELECT class FROM CDependsDef INNER JOIN Classes ON CDependsDef.class=Classes.id WHERE base=? ORDER BY Classes.k");
                return s.vec<Class>();
            }

            std::vector<Class>           def_derived_unsorted(Class c)
            {   
                static thread_local SQ s("SELECT class FROM CDependsDef WHERE base=?");
                return s.vec<Class>();
            }
        }
        
        std::vector<Class>                def_derived(Class c, Sorted sorted)
        {
            return sorted ? def_derived_sorted(c) : def_derived_unsorted(c);
        }

        namespace {
            std::vector<Field>           def_fields_sorted(Class c)
            {
                static thread_local SQ s("SELECT field FROM FDefClass INNER JOIN Fields ON FDefClass.field=Fields.id WHERE field=? ORDER BY Fields.cK");
                return s.vec<Field>(c.id);
            }

            std::vector<Field>           def_fields_unsorted(Class c)
            {
                static thread_local SQ s("SELECT field FROM FDefClass WHERE class=?");
                return s.vec<Field>(c.id);
            }
        }
        
        std::vector<Field>           def_fields(Class c, Sorted sorted)
        {
            return sorted ? def_fields_sorted(c) : def_fields_unsorted(c);
        }


        namespace {
            std::vector<Class>        def_reverse_sorted(Class c)
            {
                static thread_local SQ s("SELECT reverse FROM CReverseDef INNER JOIN Classes ON CReverseDef.reverse=Classes.id WHERE class=? ORDER BY Classes.k");
                return s.vec<Class>(c.id);
            }

            std::vector<Class>        def_reverse_unsorted(Class c)
            {
                static thread_local SQ s("SELECT reverse FROM CReverseDef WHERE class=?");
                return s.vec<Class>(c.id);
            }
        }
        
        std::vector<Class>           def_reverse(Class c, Sorted sorted)
        {   
            return sorted ? def_reverse_sorted(c) : def_reverse_unsorted(c);
        }
        
        namespace {
            std::vector<Class>        def_source_sorted(Class c)
            {
                static thread_local SQ s("SELECT source FROM CSourceDef INNER JOIN Classes ON CSourceDef.source=Classes.id WHERE class=? ORDER BY Classes.k");
                return s.vec<Class>(c.id);
            }

            std::vector<Class>        def_source_unsorted(Class c)
            {
                static thread_local SQ s("SELECT source FROM CSourceDef WHERE class=?");
                return s.vec<Class>(c.id);
            }
        }

        std::vector<Class>           def_source(Class c, Sorted sorted)
        {
            return sorted ? def_source_sorted(c) : def_source_unsorted(c);
        }
        
        namespace {
            std::vector<Class>        def_target_sorted(Class c)
            {
                static thread_local SQ s("SELECT target FROM CTargetDef INNER JOIN Classes ON CTargetDef.target=Classes.id WHERE class=? ORDER BY Classes.k");
                return s.vec<Class>(c.id);
            }

            std::vector<Class>        def_target_unsorted(Class c)
            {
                static thread_local SQ s("SELECT target FROM CTargetDef WHERE class=?");
                return s.vec<Class>(c.id);
            }
        }
        
        
        std::vector<Class>           def_target(Class c, Sorted sorted)
        {
            return sorted ? def_target_sorted(c) : def_target_unsorted(c);
        }

        namespace {
            std::vector<Class>        def_use_sorted(Class c)
            {
                static thread_local SQ s("SELECT base FROM CDependsDef INNER JOIN Classes ON CDependsDef.base=Classes.id WHERE class=? ORDER BY Classes.k");
                return s.vec<Class>(c.id);
            }

            std::vector<Class>        def_use_unsorted(Class c)
            {
                static thread_local SQ s("SELECT base FROM CDependsDef WHERE class=?");
                return s.vec<Class>(c.id);
            }
        }

        std::vector<Class>           def_use(Class c, Sorted sorted)
        {
            return sorted ? def_use_sorted(c) : def_use_unsorted(c);
        }

        // disabled until graphs are working
        //Graph               dep_graph(Class c)
        //{   
            //static thread_local SQ    s("SELECT deps FROM Classes WHERE id=?");
            //return s.as<Graph>(c.id);
        //}
        
        namespace {
            std::vector<Class>        dependents_sorted(Class c)
            {
                static thread_local SQ    s("SELECT class FROM CDepends INNER JOIN Classes ON CDepends.class=Classes.id WHERE base=? ORDER BY Classes.k");
                return s.vec<Class>(c.id);
            }
            
            std::vector<Class>        dependents_unsorted(Class c)
            {
                static thread_local SQ    s("SELECT class FROM CDepends WHERE base=?");
                return s.vec<Class>(c.id);
            }
        }
        

        std::vector<Class>       dependents(Class c, Sorted sorted)
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
        

        bool                edge(Class c)
        {
            static thread_local SQ    s("SELECT edge FROM Classes WHERE id=?");
            return s.boolean(c.id);
        }
        
        namespace {
            std::vector<Class>    edges_in_sorted(Class c)
            {
                static thread_local SQ    s("SELECT class FROM CTargets INNER JOIN Classes ON CTargets.class=Classes.id WHERE target=? ORDER BY Classes.K");
                return s.vec<Class>(c.id);
            }

            std::vector<Class>    edges_in_unsorted(Class c)
            {
                static thread_local SQ    s("SELECT class FROM CTargets WHERE target=?");
                return s.vec<Class>(c.id);
            }
        }
        

        std::vector<Class>      edges_in(Class c, Sorted sorted)
        {
            return sorted ? edges_in_sorted(c) : edges_in_unsorted(c);
        }
        
        namespace {
            std::vector<Class>    edges_out_sorted(Class c)
            {
                static thread_local SQ    s("SELECT class FROM CSources INNER JOIN Classes ON CSources.class=Classes.id WHERE source=? ORDER BY Classes.K");
                return s.vec<Class>(c.id);
            }
            
            std::vector<Class>    edges_out_unsorted(Class c)
            {
                static thread_local SQ    s("SELECT class FROM CSources WHERE source=?");
                return s.vec<Class>(c.id);
            }
        }
        
        std::vector<Class>      edges_out(Class c, Sorted sorted)
        {
            return sorted ? edges_out_sorted(c) : edges_out_unsorted(c);
        }

        void                erase(Class x)
        {
            static thread_local SQ stmts[] = {
                SQ("DELETE FROM CAlias WHERE class=?"),
                SQ("DELETE FROM CPrefix WHERE class=?"),
                SQ("DELETE FROM CSuffix WHERE class=?"),
                SQ("DELETE FROM CTags WHERE class=?"),
                SQ("DELETE FROM Classes WHERE id=?")
            };
            
            if(!x)
                return ;
                
            for(auto& sq : stmts)
                sq.exec(x.id);
        }

        bool                exists(Class c)
        {
            return exists_class(c.id);
        }
        
        

        bool                exists_class(uint64_t i)
        {
            static thread_local SQ s("SELECT 1 FROM Classes WHERE id=?");
            return s.present(i);
        }
        
        Field               field(Class c, std::string_view k)
        {
            static thread_local SQ s("SELECT id FROM Fields WHERE class=? AND k=?");
            return s.as<Field>(c.id, k);
        }
        
        namespace {
            std::vector<Field>    fields_sorted(Class c)
            {
                static thread_local SQ    s("SELECT field FROM CFields INNER JOIN Fields ON CFields.field=Fields.id WHERE class=? ORDER BY Fields.K");
                return s.vec<Field>(c.id);
            }

            std::vector<Field>    fields_unsorted(Class c)
            {
                static thread_local SQ    s("SELECT field FROM CFields WHERE class=?");
                return s.vec<Field>(c.id);
            }
        }
        

        std::vector<Field>       fields(Class c, Sorted sorted)
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
        
        namespace {
            std::vector<Class>        inbound_sorted(Class c)
            {
                static thread_local SQ s("SELECT class FROM CTargets INNER JOIN Classes ON CTargets.cleass=Classes.id WHERE target=? ORDER BY Classes.K");
                return s.vec<Class>(c.id);
            }
            
            std::vector<Class>        inbound_unsorted(Class c)
            {
                static thread_local SQ s("SELECT class FROM CTargets WHERE target=?");
                return s.vec<Class>(c.id);
            }
        }
        
        std::vector<Class>           inbound(Class c, Sorted sorted)
        {
            return sorted ? inbound_sorted(c) : inbound_unsorted(c);
        }
        
        Class::Info         info(Class c, bool autoKey)
        {
            Class::Info    ret;
            static thread_local SQ    s("SELECT k, edge, name, plural, brief, icon, deps, category, binding FROM Classes WHERE id=?");
            auto s_af = s.af();
            s.bind(1, c.id);
            if(s.step() == SqlQuery::Row){
                ret.key         = s.v_string(1);
                ret.doc         = Document(c.id);
                ret.edge        = s.v_bool(2);
                ret.name        = s.v_string(3);
                ret.plural      = s.v_string(4);
                ret.brief       = s.v_string(5);
                ret.icon        = Image(s.v_uint64(6));
                //ret.deps        = Graph(s.v_uint64(7));
                if(autoKey && ret.name.empty())
                    ret.name    = ret.key;
                ret.category    = Category(s.v_uint64(8));
                ret.binding     = s.v_string(9);
            }
            return ret;
        }

        bool                is(Class d, Class b)
        {
            static thread_local SQ    s("SELECT 1 FROM CDepends WHERE class=? AND base=?");
            return s.present(d.id, b.id);
        }
        
        
        bool                is_all(Class d, std::initializer_list<Class> b)
        {
            for(Class c : b)
                if(!is(d, c))
                    return false;
            return true;
        }

        bool                is_any(Class d, std::initializer_list<Class> b)
        {
            for(Class c : b)
                if(is(d, c))
                    return true;
            return false;
        }



        
        std::string             key(Class c)
        {
            static thread_local SQ    s("SELECT k FROM Classes WHERE id=? LIMIT 1");
            return s.str(c.id);
        }
        
        
        std::string             label(Class c)
        {
            static thread_local SQ    s("SELECT ifnull(name,k) FROM Classes WHERE id=?");
            return s.str(c.id);
        }


        Class                make_class(std::string_view k, const Root* rt)
        {
            if(!rt)
                rt      = wksp::root_first(DataRole::Config);
            if(!rt){
                yError() << "No root specified to create the class in!";
                return Class{};
            }
            
            std::string cfn = class_filename(k);
            Document        doc = db_document(classes_folder(), cfn);
            bool            was = false;
            Class           c   = db_class(doc, &was);
            if(!was)
                return c;
                
            if(fragments_count(doc))
                return c;
            Class::SharedFile    td  = writable(c, rt, DONT_LOCK);
            td -> name      = k;
            td -> plural    = td->name + 's';
            td -> save();
            return c;
        }
        

        Class::SharedData            merged(Class c, unsigned int opts)
        {
            Class::SharedData        ret = std::make_shared<Class::Data>();;
            for(auto& i : reads(c, opts)){
                if(opts & IS_UPDATE)
                    update(i.first);
                ret->merge(*(i.second), static_cast<bool>(opts&OVERRIDE));
            }
            return ret;
        }
        


        std::string             name(Class c)
        {
            static thread_local SQ    s("SELECT name FROM Classes WHERE id=?");
            return s.str(c.id);
        }

        
        NKI                 nki(Class c, bool autoKey)
        {
            static thread_local SQ    s("SELECT name,icon,k FROM Classes WHERE id=?");
            auto s_af = s.af();
            s.bind(1, c.id);
            if(s.step() == SqlQuery::Row){
                NKI  ret;
                ret.name    = s.v_string(1);
                ret.icon    = Image{s.v_uint64(2)};
                ret.key     = s.v_string(3);
                if(autoKey && ret.name.empty())
                    ret.name    = ret.key;
                return ret;
            }
            return NKI();
        }
        
        
        namespace {
            std::vector<Class>    outbound_sorted(Class c)
            {
                static thread_local SQ s("SELECT class FROM CSources INNER JOIN Classes ON CSources.class=Classes.id WHERE source=? ORDER BY Classes.K");
                return s.vec<Class>(c.id);
            }

            std::vector<Class>    outbound_unsorted(Class c)
            {
                static thread_local SQ s("SELECT class FROM CSources WHERE source=?");
                return s.vec<Class>(c.id);
            }
        }

        std::vector<Class>           outbound(Class c, Sorted sorted)
        {
            return sorted ? outbound_sorted(c) : outbound_unsorted(c);
        }

        
        std::string             plural(Class c)
        {
            static thread_local SQ    s("SELECT plural FROM Classes WHERE id=?");
            return s.str(c.id);
        }

        string_set_t            prefixes(Class c)
        {
            static thread_local SQ  s("SELECT prefix FROM CPrefix WHERE class=?");
            return s.sset(c.id);
        }

        Class::SharedFile        read(Class c, const Root*rt, unsigned int opts)
        {
            return class_doc(fragment(document(c), rt), opts);
        }

        std::vector<ClassFragDoc>    reads(Class c, unsigned int opts)
        {
            std::vector<ClassFragDoc>  ret;
            for(Fragment f : fragments(document(c), DataRole::Config)){
                Class::SharedFile    p   = class_doc(f, opts);
                if(p)
                    ret.push_back(ClassFragDoc(f, p));
            }
            return ret;
        }

        std::vector<ClassFragDoc>  reads(Class c, class Root*rt, unsigned int opts)
        {
            std::vector<ClassFragDoc>  ret;
            for(Fragment f : fragments(document(c), rt)){
                Class::SharedFile    p   = class_doc(f, opts);
                if(p)
                    ret.push_back(ClassFragDoc(f, p));
            }
            return ret;
        }
        
        namespace {
            std::vector<Class>    reverses_sorted(Class c)
            {
                static thread_local SQ    s("SELECT reverse FROM CReverses INNER JOIN Classes ON CReverses.reverse=Classes.id WHERE class=? ORDER BY Classes.K");
                return s.vec<Class>(c.id);
            }

            std::vector<Class>    reverses_unsorted(Class c)
            {
                static thread_local SQ    s("SELECT reverse FROM CReverses WHERE class=?");
                return s.vec<Class>(c.id);
            }
        }
        
        
        std::vector<Class>       reverses(Class c, Sorted sorted)
        {
            return sorted ? reverses_sorted(c) : reverses_unsorted(c);
        }
        
        namespace {
            std::vector<Class>    sources_sorted(Class c)
            {
                static thread_local SQ    s("SELECT source FROM CSources INNER JOIN Classes ON CSources.source=Classes.id WHERE class=? ORDER BY Classes.K");
                return s.vec<Class>(c.id);
            }

            std::vector<Class>    sources_unsorted(Class c)
            {
                static thread_local SQ    s("SELECT source FROM CSources WHERE class=?");
                return s.vec<Class>(c.id);
            }
        }

        std::vector<Class>  sources(Class c, Sorted sorted)
        {
            return sorted ? sources_sorted(c) : sources_unsorted(c);
        }

        size_t  sources_count(Class c)
        {
            static thread_local SQ    s("SELECT COUNT(1) FROM CSources WHERE class=?");
            return s.size(c.id);
        }

        string_set_t            suffixes(Class c)
        {
            static thread_local SQ  s("SELECT suffix FROM CSuffix WHERE class=?");
            return s.sset(c.id);
        }

        std::set<Tag>               tag_set(Class c)
        {
            static thread_local SQ s("SELECT tag FROM CTags WHERE class=?");
            return s.set<Tag>(c.id);
        }

        bool  tagged(Class c, Tag t)
        {
            static thread_local SQ s("SELECT 1 FROM CTags WHERE class=? AND tag=?");
            return s.present(c.id, t.id);
        }

        namespace {
            std::vector<Tag>  tags_sorted(Class c)
            {
                static thread_local SQ s("SELECT tag FROM CTags INNER JOIN Tags ON CTags.tag=Tags.id WHERE class=? ORDER BY Tags.K");
                return s.vec<Tag>(c.id);
            }

            std::vector<Tag>  tags_unsorted(Class c)
            {
                static thread_local SQ s("SELECT tag FROM CTags WHERE class=?");
                return s.vec<Tag>(c.id);
            }
        }

        std::vector<Tag>  tags(Class c, Sorted sorted)
        {
            return sorted ? tags_sorted(c) : tags_unsorted(c);
        }
        
        
        size_t  tags_count(Class c)
        {
            static thread_local SQ s("SELECT COUNT(1) FROM CTags WHERE class=?");
            return s.size(c.id);
        }


        namespace {
            std::vector<Class>    targets_sorted(Class c)
            {
                static thread_local SQ    s("SELECT target FROM CTargets INNER JOIN Classes ON CTargets.target=Classes.id WHERE class=? ORDER BY Classes.k");
                return s.vec<Class>(c.id);
            }

            std::vector<Class>    targets_unsorted(Class c)
            {
                static thread_local SQ    s("SELECT target FROM CTargets WHERE class=?");
                return s.vec<Class>(c.id);
            }
        }
        
        std::vector<Class>       targets(Class c, Sorted sorted)
        {
            return sorted ? targets_sorted(c) : targets_unsorted(c);
        }

        size_t          targets_count(Class c)
        {
            static thread_local SQ    s("SELECT COUNT(1) FROM CTargets WHERE class=?");
            return s.size(c.id);
        }
        
        void                update_icon(Class x)
        {
            Document    doc     = document(x);
            Image       img     = best_image(doc);
            static thread_local SQ u1("UPDATE Classes SET icon=? WHERE id=?");
            u1.exec(img.id, x.id);
            static thread_local SQ u2("UPDATE Documents SET icon=? WHERE id=?");
            u2.exec(doc.id, x.id);
        }


        Class::SharedData           update_info(Class x, unsigned int opts)
        {
            Class::SharedData data = merged(x, IS_UPDATE | opts);
            if(!data){
                yWarning() << "Unable to update class '" << key(x) << "' due to lack of data";
                return {};
            }
            
            Category cat = category(data->category);
            static thread_local SQ uc("UPDATE Classes SET name=?, plural=?, brief=?, category=?, binding=? WHERE id=?");
            uc.bind(1, data->name);
            uc.bind(2, data->plural);
            uc.bind(3, data->brief);
            uc.bind(4, cat.id);
            uc.bind(5, data->binding);
            uc.bind(6, x.id);
            uc.exec();
            return data;
        }
        
        
        namespace {
            std::vector<Class>    uses_sorted(Class c)
            {
                static thread_local SQ    s("SELECT base FROM CDepends INNER JOIN Classes ON CDepends.base=Classes.id WHERE class=? ORDER BY Classes.k");
                return s.vec<Class>(c.id);
            }

            std::vector<Class>    uses_unsorted(Class c)
            {
                static thread_local SQ    s("SELECT base FROM CDepends WHERE class=?");
                return s.vec<Class>(c.id);
            }
        }
        

        std::vector<Class>   uses(Class c, Sorted sorted)
        {
            return sorted ? uses_sorted(c) : uses_unsorted(c);
        }
        
        size_t              uses_count(Class c)
        {
            static thread_local SQ s("SELECT COUNT(1) FROM CDepends WHERE class=?");
            return s.size(c.id);
        }
        
        Class::SharedFile        writable(Class c, const Root* rt, unsigned int opts)
        {
            Document    d   = document(c);
            if(!d)
                return Class::SharedFile();
            if(rt && !rt->is_writable(DataRole::Config))
                return Class::SharedFile();

            Fragment    f   = fragment(d);
            if(f)
                return class_doc(f, opts | ALLOW_EMPTY);
            Folder      fo  = folder(d);
            if((fo != cdb::top_folder()) && !exists(fo, rt))
                make_directory(fo, rt);

            Class::SharedFile ptr  = std::make_shared<Class::File>();
            ptr -> set_file( rt -> resolve(key(d)));
            ptr -> reload();
            return ptr;
        }


    }
}
