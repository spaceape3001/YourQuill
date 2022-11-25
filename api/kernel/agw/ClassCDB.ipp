////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ClassCDB.hpp"
#include <basic/ByteArray.hpp>
#include <basic/CollectionUtils.hpp>
#include <basic/FileUtils.hpp>
#include <kernel/agw/ClassFile.hpp>
#include <kernel/agw/Field.hpp>
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
            inline std::string class_filename(std::string_view k)
            {
                return make_filename(k, agw::Class::szExtension);
            }
            
            inline std::vector<agw::Class::Rank>     exec_class_rank_vector(SQ& s)
            {
                std::vector<agw::Class::Rank>    ret;
                while(s.step() == SqlQuery::Row)
                    ret.push_back({ agw::Class(s.v_uint64(1)), s.v_int64(2)});
                s.reset();
                return ret;
            }
        }


        agw::ClassVector  all_classes(Sorted sorted)
        {
            static thread_local SQ    qs("SELECT id FROM Classes ORDER BY K");
            static thread_local SQ    qu("SELECT id FROM Classes");
            SQ& s = sorted ? qs : qu;
            return s.vec<agw::Class>();
        }

        size_t  all_classes_count()
        {
            static thread_local SQ    s("SELECT COUNT(1) FROM Classes");
            return s.size();
        }

        string_set_t        alternative_keys(agw::Class c)
        {
            static thread_local SQ   s("SELECT k FROM CLookup WHERE class=? AND priority=0");
            return s.sset(c.id);
        }
        
        agw::ClassVector   base_classes(agw::Class c, Sorted sorted)
        {
            static thread_local SQ    qs("SELECT base FROM CDepends INNER JOIN Classes ON CDepends.base=Classes.id WHERE class=? ORDER BY Classes.k");
            static thread_local SQ    qu("SELECT base FROM CDepends WHERE class=?");
            SQ& s = sorted ? qs : qu;
            return s.vec<agw::Class>(c.id);
        }
        
        size_t              base_classes_count(agw::Class c)
        {
            static thread_local SQ s("SELECT COUNT(1) FROM CDepends WHERE class=?");
            return s.size(c.id);
        }
        
        std::vector<agw::Class::Rank>    base_classes_ranked(agw::Class c, Sorted sorted)
        {
            static thread_local SQ  qs("SELECT base,hops FROM CDepends INNER JOIN Classes ON CDepends.base=Classes.id WHERE class=? ORDER BY hops,Classes.k");
            static thread_local SQ  qu("SELECT base,hops FROM CDepends WHERE class=?");
            SQ& s = sorted ? qs : qu;
            s.bind(1, c.id);
            return exec_class_rank_vector(s);
        }

        std::vector<agw::Class::Rank>    base_classes_ranked_limited(agw::Class c, uint64_t maxDepth, Sorted sorted)
        {
            static thread_local SQ  qs("SELECT base,hops FROM CDepends INNER JOIN Classes ON CDepends.base=Classes.id WHERE class=? AND hops<=? ORDER BY hops,Classes.k");
            static thread_local SQ  qu("SELECT base,hops FROM CDepends WHERE class=? AND hops<=?");
            SQ& s   = sorted ? qs : qu;
            s.bind(1, c.id);
            s.bind(2, maxDepth);
            return exec_class_rank_vector(s);
        }

        agw::ClassCountMap               base_classes_ranked_merged_map(const agw::ClassSet&cset, bool inSet)
        {
            agw::ClassCountMap   ret;
            for(agw::Class c : cset){
                if(inSet)
                    ret[c]  = { 0 };
                for(agw::Class::Rank cr : base_classes_ranked(c))
                    ret[cr.cls].set_smaller(HCountU64{(uint64_t) cr.rank});
            }
            return ret;
        }

        std::string  binding(agw::Class c)
        {
            static thread_local SQ  s("SELECT binding FROM Classes WHERE id=?");
            return s.str(c.id);
        }

        std::string  brief(agw::Class c)
        {
            static thread_local SQ    s("SELECT brief FROM Classes WHERE id=?");
            return s.str(c.id);
        }
        
        Category  category(agw::Class c)
        {
            static thread_local SQ s("SELECT category FROM Classes WHERE id=?");
            return s.as<Category>(c.id);
        }

        agw::Class  class_(uint64_t i)
        {
            return exists_class(i) ? agw::Class{i} : agw::Class();
        }

        agw::Class  class_(Document doc, bool calc)
        {
            if(!doc)
                return agw::Class();
            if(exists_class(doc.id))
                return agw::Class{doc.id};
            if(calc && (folder(doc) == classes_folder())){
                std::string k   = base_key(doc);
                return class_(k);
            }
            
            return agw::Class();
        }
        
        agw::Class  class_(std::string_view  k)
        {
            static thread_local SQ s("SELECT class, priority FROM CLookup WHERE k=? ORDER BY priority DESC LIMIT 1");
            return s.as<agw::Class>(k);
        }

        agw::Class::SharedFile  class_doc(Fragment f, cdb_options_t opts)
        {
            if(!f)
                return agw::Class::SharedFile();
                
            std::filesystem::path       fp  = path(f);
            Fragment::Lock  lk;
            if(!(opts & DONT_LOCK)){
                lk  = Fragment::Lock::read(f);
                if(!lk){
                    yWarning() << "Unable to get read lock on fragment: " << fp;
                    return agw::Class::SharedFile();
                }
            }

            auto ch   = file_bytes(fp);
            lk.free();
            if(ch.empty()){
                if(opts & ALLOW_EMPTY)
                    return std::make_shared<agw::Class::File>();
                return agw::Class::SharedFile();
            }
                
            agw::Class::SharedFile  td = std::make_shared<agw::Class::File>();
            if(!td->load(std::move(ch), fp)){
                yError() << "Unable to read " << fp;
                return agw::Class::SharedFile();
            }
            td -> set_file(fp);
            return td;
        }
        

        agw::ClassVector       classes(const string_set_t& sset, bool noisy)
        {
            agw::ClassVector   ret;
            for(const std::string& s : sset){
                if(s.empty())
                    continue;
                    
                agw::Class cls = class_(s);
                if(!cls){
                    if(noisy)
                        yWarning() << "Unable to find class: " << s;
                    continue;
                }
                
                ret.push_back(cls);
            }
            return ret;
        }

        std::set<agw::Class>     classes_set(const string_set_t&sset, bool noisy)
        {
            std::set<agw::Class>   ret;
            for(const std::string& s : sset){
                if(s.empty())
                    continue;
                    
                agw::Class cls = class_(s);
                if(!cls){
                    if(noisy)
                        yWarning() << "Unable to find class: " << s;
                    continue;
                }
                
                ret.insert(cls);
            }
            return ret;
        }
        
        std::set<agw::Class>     classes_set(const string_view_set_t&sset, bool noisy)
        {
            std::set<agw::Class>   ret;
            for(std::string_view s : sset){
                if(s.empty())
                    continue;
                    
                agw::Class cls = class_(s);
                if(!cls){
                    if(noisy)
                        yWarning() << "Unable to find class: " << s;
                    continue;
                }
                
                ret.insert(cls);
            }
            return ret;
        }
        
        agw::ClassVector          classes_with_tag(Tag x, Sorted sorted)
        {
            static thread_local SQ qs("SELECT class FROM CTags INNER JOIN Classes ON CTags.class=Classes.id WHERE tag=? ORDER BY Classes.k");
            static thread_local SQ qu("SELECT class FROM CTags WHERE tag=?");
            SQ& s = sorted ? qs : qu;
            return s.vec<agw::Class>(x.id);
        }

        agw::Class               db_class(std::string_view k, bool *wasCreated)
        {
            std::string     tfn = class_filename(k);
            return db_class(db_document(classes_folder(), tfn), wasCreated);
        }
        
        agw::Class               db_class(Document doc, bool *wasCreated)
        {
            if(wasCreated)
                *wasCreated = false;
            if(folder(doc) != classes_folder())
                return agw::Class{};
            std::string k   = base_key(doc);
            if(k.empty())
                return agw::Class();
        
            static thread_local SQ    i("INSERT OR FAIL INTO Classes (id,k) VALUES (?,?)");
            static thread_local SQ    i2("INSERT OR REPLACE INTO CLookup (class,k,priority) VALUES (?,?,1)");
            auto i_lk   = i.af();

            i.bind(1, doc.id);
            i.bind(2, k);
            if(i.exec()){
                if(wasCreated)
                    *wasCreated = true;
                i2.bind(1, doc.id);
                i2.bind(2, k);
                i2.exec();
                return agw::Class{doc.id};
            } else if(exists_class(doc.id)){
                return agw::Class{doc.id};
            } else {
                yError() << "Unable to get the class from the database: " << k;
                return agw::Class();
            }
        }

        agw::ClassVector            db_classes(const string_set_t& all)
        {
            agw::ClassVector   ret;
            ret.reserve(all.size());
            for(std::string_view  s: all)
                ret.push_back(db_class(s));
            return ret;
        }

        agw::ClassVector             db_classes(const string_view_set_t& all)
        {
            agw::ClassVector   ret;
            ret.reserve(all.size());
            for(std::string_view  s: all)
                ret.push_back(db_class(s));
            return ret;
        }
        
        #if 0

        string_set_t         def_aliases(agw::Class c)
        {
            static thread_local SQ s("SELECT alias FROM CAlias WHERE class=?");
            return s.sset(c.id);
        }
        
        namespace {
            agw::ClassVector           def_derived_sorted(agw::Class c)
            {   
                static thread_local SQ s("SELECT class FROM CDepends INNER JOIN Classes ON CDepends.class=Classes.id WHERE base=? AND hops=0 ORDER BY Classes.k");
                return s.vec<agw::Class>();
            }

            agw::ClassVector           def_derived_unsorted(agw::Class c)
            {   
                static thread_local SQ s("SELECT class FROM CDepends WHERE base=? AND hops=0 ");
                return s.vec<agw::Class>();
            }
        }
        
        agw::ClassVector                def_derived(agw::Class c, Sorted sorted)
        {
            return sorted ? def_derived_sorted(c) : def_derived_unsorted(c);
        }

        namespace {
            std::vector<Field>           def_fields_sorted(agw::Class c)
            {
                static thread_local SQ s("SELECT field FROM CFields INNER JOIN Fields ON CFields.field=Fields.id WHERE CFields.class=? AND hops=0 ORDER BY Fields.cK");
                return s.vec<Field>(c.id);
            }

            std::vector<Field>           def_fields_unsorted(agw::Class c)
            {
                static thread_local SQ s("SELECT field FROM CFields WHERE class=? AND hops=0");
                return s.vec<Field>(c.id);
            }
        }
        
        std::vector<Field>           def_fields(agw::Class c, Sorted sorted)
        {
            return sorted ? def_fields_sorted(c) : def_fields_unsorted(c);
        }

        string_set_t            def_prefixes(agw::Class c)
        {
            static thread_local SQ  s("SELECT prefix FROM CPrefix WHERE class=?");
            return s.sset(c.id);
        }

        namespace {
            agw::ClassVector        def_reverse_sorted(agw::Class c)
            {
                static thread_local SQ s("SELECT reverse FROM CReverses INNER JOIN Classes ON CReverseDef.reverse=Classes.id WHERE class=? AND hops=0 ORDER BY Classes.k");
                return s.vec<agw::Class>(c.id);
            }

            agw::ClassVector        def_reverse_unsorted(agw::Class c)
            {
                static thread_local SQ s("SELECT reverse FROM CReverses WHERE class=? AND hops=0");
                return s.vec<agw::Class>(c.id);
            }
        }
        
        agw::ClassVector           def_reverse(agw::Class c, Sorted sorted)
        {   
            return sorted ? def_reverse_sorted(c) : def_reverse_unsorted(c);
        }
        
        namespace {
            agw::ClassVector        def_source_sorted(agw::Class c)
            {
                static thread_local SQ s("SELECT source FROM CSources INNER JOIN Classes ON CSourceDef.source=Classes.id WHERE class=? AND hops=0 ORDER BY Classes.k");
                return s.vec<agw::Class>(c.id);
            }

            agw::ClassVector        def_source_unsorted(agw::Class c)
            {
                static thread_local SQ s("SELECT source FROM CSources WHERE class=? AND hops=0");
                return s.vec<agw::Class>(c.id);
            }
        }

        agw::ClassVector           def_source(agw::Class c, Sorted sorted)
        {
            return sorted ? def_source_sorted(c) : def_source_unsorted(c);
        }
        
        string_set_t                def_suffixes(agw::Class c)
        {
            static thread_local SQ  s("SELECT suffix FROM CSuffix WHERE class=?");
            return s.sset(c.id);
        }
        
        agw::ClassVector           def_target(agw::Class c, Sorted sorted)
        {
            static thread_local SQ qs("SELECT target FROM CTargets INNER JOIN Classes ON CTargetDef.target=Classes.id WHERE class=? AND hops=0 ORDER BY Classes.k");
            static thread_local SQ qu("SELECT target FROM CTargets WHERE class=? AND hops=0");
            SQ& s = sorted ? qs : qu;
            return s.vec<agw::Class>(c.id);
        }

        agw::ClassVector           def_use(agw::Class c, Sorted sorted)
        {
            static thread_local SQ qs("SELECT base FROM CDepends INNER JOIN Classes ON CDepends.base=Classes.id WHERE class=? AND hops=0 ORDER BY Classes.k");
            static thread_local SQ qu("SELECT base FROM CDepends WHERE class=? AND hops=0");
            SQ& s = sorted ? qs : qu;
            return s.vec<agw::Class>(c.id);
        }
        
        #endif
        

        // disabled until graphs are working
        //Graph               dep_graph(agw::Class c)
        //{   
            //static thread_local SQ    s("SELECT deps FROM Classes WHERE id=?");
            //return s.as<Graph>(c.id);
        //}
        
        agw::ClassVector       derived_classes(agw::Class c, Sorted sorted)
        {
            static thread_local SQ    qs("SELECT class FROM CDepends INNER JOIN Classes ON CDepends.class=Classes.id WHERE base=? ORDER BY Classes.k");
            static thread_local SQ    qu("SELECT class FROM CDepends WHERE base=?");
            SQ& s = sorted ? qs : qu;
            return s.vec<agw::Class>(c.id);
        }
        
        std::vector<agw::Class::Rank>    derived_classes_ranked(agw::Class c, Sorted sorted)
        {
            static thread_local SQ  qs("SELECT class, hops FROM CDepends INNER JOIN Classes ON CDepends.class=Classes.id WHERE base=? ORDER BY hops, Classes.k");
            static thread_local SQ  qu("SELECT class, hops FROM CDepends WHERE base=?");
            SQ& s = sorted ? qs : qu;
            s.bind(1, c.id);
            return exec_class_rank_vector(s);
        }
        
        std::vector<agw::Class::Rank>    derived_classes_limited_ranked(agw::Class c, uint64_t maxDepth, Sorted sorted)
        {
            static thread_local SQ qs("SELECT class, hops FROM CDepends INNER JOIN Classes ON CDepends.class=Classes.id WHERE base=? AND hops<=? ORDER BY hops, Classes.k");
            static thread_local SQ qu("SELECT class, hops FROM CDepends WHERE base=? AND hops<=?");
            
            SQ& s = sorted ? qs : qu;
            s.bind(1, c.id);
            s.bind(2, maxDepth);
            return exec_class_rank_vector(s);
        }

        agw::ClassCountMap               derived_classes_ranked_merged_map(const agw::ClassSet&cset, bool inSet)
        {
            agw::ClassCountMap   ret;
            for(agw::Class c : cset){
                if(inSet)
                    ret[c]  = { 0 };
                for(agw::Class::Rank cr : derived_classes_ranked(c))
                    ret[cr.cls].set_smaller(HCountU64{(uint64_t) cr.rank});
            }
            return ret;
        }


        Folder              detail_folder(agw::Class c)
        {
            return db_folder( classes_folder(), key(c));
        }

        Document            document(agw::Class c)
        {
            return exists(c) ? Document{c.id}  : Document{};
        }
        

        
        agw::ClassVector      edge_classes_in(agw::Class c, Sorted sorted)
        {
            static thread_local SQ qs("SELECT class FROM CTargets INNER JOIN Classes ON CTargets.class=Classes.id WHERE target=? ORDER BY Classes.K");
            static thread_local SQ qu("SELECT class FROM CTargets WHERE target=?");
            SQ& s = sorted ? qs : qu;
            return s.vec<agw::Class>(c.id);
        }
        
        agw::ClassVector      edge_classes_out(agw::Class c, Sorted sorted)
        {
            static thread_local SQ qs("SELECT class FROM CSources INNER JOIN Classes ON CSources.class=Classes.id WHERE source=? ORDER BY Classes.K");
            static thread_local SQ qu("SELECT class FROM CSources WHERE source=?");
            SQ& s = sorted ? qs : qu;
            return s.vec<agw::Class>(c.id);
        }


        bool                exists(agw::Class c)
        {
            return exists_class(c.id);
        }
        
        

        bool                exists_class(uint64_t i)
        {
            static thread_local SQ s("SELECT 1 FROM Classes WHERE id=?");
            return s.present(i);
        }
        
        agw::Field               field(agw::Class c, std::string_view k)
        {
            static thread_local SQ s("SELECT id FROM Fields WHERE class=? AND k=?");
            return s.as<agw::Field>(c.id, k);
        }
        
        agw::FieldVector        fields(agw::Class c, Sorted sorted)
        {
            static thread_local SQ  qs("SELECT field FROM CFields INNER JOIN Fields ON CFields.field=Fields.id WHERE CFields.class=? ORDER BY Fields.K");
            static thread_local SQ  qu("SELECT field FROM CFields WHERE class=?");
            SQ& s = sorted ? qs : qu;
            return s.vec<agw::Field>(c.id);
        }
        
        size_t                  fields_count(agw::Class c)
        {
            static thread_local SQ    s("SELECT COUNT(1) FROM CFields WHERE class=?");
            return s.size(c.id);
        }
        
        
        Image                   icon(agw::Class c)
        {
            static thread_local SQ    s("SELECT icon FROM Classes WHERE id=? LIMIT 1");
            return s.as<Image>(c.id);
        }
        
        agw::ClassVector           inbound_classes(agw::Class c, Sorted sorted)
        {
            static thread_local SQ qs("SELECT class FROM CTargets INNER JOIN Classes ON CTargets.class=Classes.id WHERE target=? ORDER BY Classes.K");
            static thread_local SQ qu("SELECT class FROM CTargets WHERE target=?");
            SQ& s = sorted ? qs : qu;
            return s.vec<agw::Class>(c.id);
        }
        
        std::vector<agw::Class::Rank>      inbound_classes_ranked(agw::Class c, Sorted sorted)
        {
            static thread_local SQ qs("SELECT class,hops_cls FROM CTargets INNER JOIN Classes ON CTargets.class=Classes.id WHERE target=? ORDER BY hops_cls,Classes.K");
            static thread_local SQ qu("SELECT class,hops_cls FROM CTargets WHERE target=?");
            SQ& s = sorted ? qs : qu;
            s.bind(1, c.id);
            return exec_class_rank_vector(s);
        }

        agw::Class::Info         info(agw::Class c, bool autoKey)
        {
            agw::Class::Info    ret;
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

        bool                is(agw::Class d, agw::Class b)
        {
            static thread_local SQ    s("SELECT 1 FROM CDepends WHERE class=? AND base=?");
            return s.present(d.id, b.id);
        }
        
        
        bool                is_all(agw::Class d, std::initializer_list<agw::Class> b)
        {
            for(agw::Class c : b)
                if(!is(d, c))
                    return false;
            return true;
        }

        bool                is_any(agw::Class d, std::initializer_list<agw::Class> b)
        {
            for(agw::Class c : b)
                if(is(d, c))
                    return true;
            return false;
        }

        bool                is_edge(agw::Class c)
        {
            static thread_local SQ    s("SELECT edge FROM Classes WHERE id=?");
            return s.boolean(c.id);
        }
        
        std::string             key(agw::Class c)
        {
            static thread_local SQ    s("SELECT k FROM Classes WHERE id=? LIMIT 1");
            return s.str(c.id);
        }
        
        
        std::string             label(agw::Class c)
        {
            std::string s   = name(c);
            if(s.empty())
                s   = key(c);
            return s;

            //static thread_local SQ    s("SELECT ifnull(name,k) FROM Classes WHERE id=?");
            //return s.str(c.id);
        }


        agw::Class                make_class(std::string_view k, const Root* rt, cdb_options_t opts, bool *wasCreated)
        {
            if(k.empty()){
                yError() << "Cannot create a BLANK class.";
                return agw::Class();
            }
            if(wasCreated)
                *wasCreated = false;
            if(!rt)
                rt      = wksp::root_first(DataRole::Config);
            if(!rt){
                yError() << "No root specified to create the class in!";
                return agw::Class{};
            }
            
            std::string cfn = class_filename(k);
            Document        doc = db_document(classes_folder(), cfn);
            bool            was = false;
            agw::Class           c   = db_class(doc, &was);
            if(!c){
                yWarning() << "Unable to create/find class: " << k;
                return c;
            }
            if(wasCreated)
                *wasCreated = was;
            if(!was)
                return c;
                
            if(fragments_count(doc))
                return c;
            agw::Class::Lock   lk;
            if(!(opts & DONT_LOCK))
                lk  = agw::Class::Lock::write(c);

            agw::Class::SharedFile    td  = writable(c, rt, DONT_LOCK);
            td -> name      = k;
            td -> plural    = td->name + 's';
            td -> save();
            return c;
        }
        
        std::map<agw::Class,HCountU64>   make_count_map(const std::vector<agw::Class::Rank>& data)
        {
            std::map<agw::Class,HCountU64>   ret;
            for(const  agw::Class::Rank& cr : data)
                ret[cr.cls]     = HCountU64{(uint64_t) cr.rank};
            return ret;
        }

        agw::Class::SharedData            merged(agw::Class c, cdb_options_t opts)
        {
            agw::Class::SharedData        ret = std::make_shared<agw::Class::Data>();;
            for(auto& i : reads(c, opts)){
                if(opts & IS_UPDATE)
                    update(i.first);
                ret->merge(*(i.second), static_cast<bool>(opts&OVERRIDE));
            }
            return ret;
        }
        

        std::string             name(agw::Class c)
        {
            static thread_local SQ    s("SELECT name FROM Classes WHERE id=?");
            return s.str(c.id);
        }

        NKI                 nki(agw::Class c, bool autoKey)
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
        
        
        agw::ClassVector           outbound_classes(agw::Class c, Sorted sorted)
        {
            static thread_local SQ qs("SELECT class FROM CSources INNER JOIN Classes ON CSources.class=Classes.id WHERE source=? ORDER BY Classes.K");
            static thread_local SQ qu("SELECT class FROM CSources WHERE source=?");
            SQ& s = sorted ? qs : qu;
            return s.vec<agw::Class>(c.id);
        }

        std::vector<agw::Class::Rank>     outbound_classes_ranked(agw::Class c, Sorted sorted)
        {
            static thread_local SQ qs("SELECT class,hops_cls FROM CSources INNER JOIN Classes ON CSources.class=Classes.id WHERE source=? ORDER BY hops_cls,Classes.K");
            static thread_local SQ qu("SELECT class,hops_cls FROM CSources WHERE source=?");
            SQ& s = sorted ? qs : qu;
            s.bind(1, c.id);
            return exec_class_rank_vector(s);
        }
        
        std::string             plural(agw::Class c)
        {
            static thread_local SQ    s("SELECT plural FROM Classes WHERE id=?");
            return s.str(c.id);
        }

        agw::Class::SharedFile        read(agw::Class c, const Root*rt, cdb_options_t opts)
        {
            return class_doc(fragment(document(c), rt), opts);
        }

        std::vector<agw::ClassFragDoc>    reads(agw::Class c, cdb_options_t opts)
        {
            std::vector<agw::ClassFragDoc>  ret;
            for(Fragment f : fragments(document(c), DataRole::Config)){
                agw::Class::SharedFile    p   = class_doc(f, opts);
                if(p)
                    ret.push_back(agw::ClassFragDoc(f, p));
            }
            return ret;
        }

        std::vector<agw::ClassFragDoc>  reads(agw::Class c, class Root*rt, cdb_options_t opts)
        {
            std::vector<agw::ClassFragDoc>  ret;
            for(Fragment f : fragments(document(c), rt)){
                agw::Class::SharedFile    p   = class_doc(f, opts);
                if(p)
                    ret.push_back(agw::ClassFragDoc(f, p));
            }
            return ret;
        }
        
        agw::ClassVector       reverses(agw::Class c, Sorted sorted)
        {
            static thread_local SQ    qs("SELECT reverse FROM CReverses INNER JOIN Classes ON CReverses.reverse=Classes.id WHERE class=? ORDER BY Classes.K");
            static thread_local SQ    qu("SELECT reverse FROM CReverses WHERE class=?");
            SQ& s   = sorted ? qs : qu;
            return s.vec<agw::Class>(c.id);
        }
        
        agw::ClassVector  sources(agw::Class c, Sorted sorted)
        {
            static thread_local SQ    qs("SELECT source FROM CSources INNER JOIN Classes ON CSources.source=Classes.id WHERE class=? ORDER BY Classes.K");
            static thread_local SQ    qu("SELECT source FROM CSources WHERE class=?");

            SQ& s   = sorted ? qs : qu;
            return s.vec<agw::Class>(c.id);
        }

        size_t  sources_count(agw::Class c)
        {
            static thread_local SQ    s("SELECT COUNT(1) FROM CSources WHERE class=?");
            return s.size(c.id);
        }
        
        std::vector<agw::Class::Rank>    source_classes_ranked(agw::Class c, Sorted sorted)
        {
            static thread_local SQ  qs("SELECT source, hops_src FROM CSources INNER JOIN Classes ON CSources.source=Classes.id WHERE class=? ORDER BY hops_src, Classes.k");
            static thread_local SQ  qu("SELECT source, hops_src FROM CSources WHERE class=?");
            SQ&     s   = sorted ? qs : qu;
            s.bind(1, c.id);
            return exec_class_rank_vector(s);
        }

        std::vector<agw::Class::Rank>    source_classes_ranked_limited(agw::Class c, uint64_t maxDepth, Sorted sorted)
        {
            static thread_local SQ  qs("SELECT source, hops_src FROM CSources INNER JOIN Classes ON CSources.source=Classes.id WHERE class=? AND hops<=? ORDER BY hops_src, Classes.k");
            static thread_local SQ  qu("SELECT source, hops_src FROM CSources WHERE class=? AND hops<=?");
            SQ&     s   = sorted ? qs : qu;
            s.bind(1, c.id);
            s.bind(2, maxDepth);
            return exec_class_rank_vector(s);
        }

        std::set<Tag>               tag_set(agw::Class c)
        {
            static thread_local SQ s("SELECT tag FROM CTags WHERE class=?");
            return s.set<Tag>(c.id);
        }

        bool  tagged(agw::Class c, Tag t)
        {
            static thread_local SQ s("SELECT 1 FROM CTags WHERE class=? AND tag=?");
            return s.present(c.id, t.id);
        }

        std::vector<Tag>  tags(agw::Class c, Sorted sorted)
        {
            static thread_local SQ qs("SELECT tag FROM CTags INNER JOIN Tags ON CTags.tag=Tags.id WHERE class=? ORDER BY Tags.K");
            static thread_local SQ qu("SELECT tag FROM CTags WHERE class=?");
            SQ& s = sorted ? qs : qu;
            return s.vec<Tag>(c.id);
        }
        
        
        size_t  tags_count(agw::Class c)
        {
            static thread_local SQ s("SELECT COUNT(1) FROM CTags WHERE class=?");
            return s.size(c.id);
        }

        agw::ClassVector       target_classes(agw::Class c, Sorted sorted)
        {
            static thread_local SQ  qs("SELECT target FROM CTargets INNER JOIN Classes ON CTargets.target=Classes.id WHERE class=? ORDER BY Classes.k");
            static thread_local SQ  qu("SELECT target FROM CTargets WHERE class=?");
            SQ& s = sorted ? qs : qu;
            return s.vec<agw::Class>(c.id);
        }

        size_t          target_classes_count(agw::Class c)
        {
            static thread_local SQ    s("SELECT COUNT(1) FROM CTargets WHERE class=?");
            return s.size(c.id);
        }

        agw::Class::SharedFile        writable(agw::Class c, const Root* rt, cdb_options_t opts)
        {
            if(!c)
                return agw::Class::SharedFile();
            Document    d   = document(c);
            if(!d){
                yWarning() << "write(agw::Class '" << key(c) << "'): Has no document!";
                return agw::Class::SharedFile();
            }
            if(rt && !rt->is_writable(DataRole::Config)){
                yWarning() << "write(agw::Class '" << key(c) << "'): Root " << rt->key << " cannot be written to!";
                return agw::Class::SharedFile();
            }

            Fragment    f   = fragment(d);
            if(f)
                return class_doc(f, opts | ALLOW_EMPTY);
            Folder      fo  = folder(d);
            if((fo != cdb::top_folder()) && !exists(fo, rt))
                make_directory(fo, rt);

            agw::Class::SharedFile ptr  = std::make_shared<agw::Class::File>();
            ptr -> set_file( rt -> resolve(key(d)));
            ptr -> reload();
            return ptr;
        }
    }

}

