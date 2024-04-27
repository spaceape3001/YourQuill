////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ClassCDB.hpp"
#include <0/basic/ByteArray.hpp>
#include <0/basic/CollectionUtils.hpp>
#include <0/io/FileUtils.hpp>
#include <mithril/class/ClassFile.hpp>
#include <mithril/field/Field.hpp>
#include <mithril/wksp/CacheQuery.hpp>
#include <mithril/document/DocumentCDB.hpp>
#include <mithril/folder/FolderCDB.hpp>
#include <mithril/fragment/FragmentCDB.hpp>
#include <mithril/root/RootDir.hpp>
#include <mithril/image/ImageCDB.hpp>
#include <mithril/category/CategoryCDB.hpp>
#include <mithril/tag/TagCDB.hpp>
#include <mithril/wksp/Workspace.hpp>

namespace yq::mithril::cdb {


    string_set_t                 aliases_set(Class c)
    {
        static thread_local CacheQuery   s("SELECT alias FROM " TBL_CLASS_ALIAS " WHERE class=?");
        return s.sset(c.id);
    }

    namespace {
        inline std::string class_filename(std::string_view k)
        {
            return make_filename(k, Class::EXTENSION);
        }
        
        inline std::vector<Class::Rank>     exec_class_rank_vector(CacheQuery& s)
        {
            std::vector<Class::Rank>    ret;
            while(s.step() == SQResult::Row)
                ret.push_back({ Class(s.v_uint64(1)), s.v_int64(2)});
            s.reset();
            return ret;
        }
    }


    std::vector<Class>  all_classes(Sorted sorted)
    {
        static thread_local CacheQuery    qs("SELECT id FROM " TBL_CLASSES " ORDER BY K");
        static thread_local CacheQuery    qu("SELECT id FROM " TBL_CLASSES "");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Class>();
    }
    
    size_t  all_classes_count()
    {
        static thread_local CacheQuery    s("SELECT COUNT(1) FROM " TBL_CLASSES "");
        return s.size();
    }

    string_set_t        alternative_keys(Class c)
    {
        static thread_local CacheQuery   s("SELECT k FROM CLookup WHERE class=? AND priority=0");
        return s.sset(c.id);
    }
    
    std::vector<Class>   base_classes(Class c, Sorted sorted)
    {
        static thread_local CacheQuery    qs("SELECT base FROM " TBL_CLASS_DEPEND " INNER JOIN " TBL_CLASSES " ON " TBL_CLASS_DEPEND ".base=" TBL_CLASSES ".id WHERE class=? ORDER BY " TBL_CLASSES ".k");
        static thread_local CacheQuery    qu("SELECT base FROM " TBL_CLASS_DEPEND " WHERE class=?");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Class>(c.id);
    }
    
    size_t              base_classes_count(Class c)
    {
        static thread_local CacheQuery s("SELECT COUNT(1) FROM " TBL_CLASS_DEPEND " WHERE class=?");
        return s.size(c.id);
    }
    
    std::vector<Class::Rank>    base_classes_ranked(Class c, Sorted sorted)
    {
        static thread_local CacheQuery  qs("SELECT base,hops FROM " TBL_CLASS_DEPEND " INNER JOIN " TBL_CLASSES " ON " TBL_CLASS_DEPEND ".base=" TBL_CLASSES ".id WHERE class=? ORDER BY hops," TBL_CLASSES ".k");
        static thread_local CacheQuery  qu("SELECT base,hops FROM " TBL_CLASS_DEPEND " WHERE class=?");
        CacheQuery& s = sorted ? qs : qu;
        s.bind(1, c.id);
        return exec_class_rank_vector(s);
    }

    std::vector<Class::Rank>    base_classes_ranked_limited(Class c, uint64_t maxDepth, Sorted sorted)
    {
        static thread_local CacheQuery  qs("SELECT base,hops FROM " TBL_CLASS_DEPEND " INNER JOIN " TBL_CLASSES " ON " TBL_CLASS_DEPEND ".base=" TBL_CLASSES ".id WHERE class=? AND hops<=? ORDER BY hops," TBL_CLASSES ".k");
        static thread_local CacheQuery  qu("SELECT base,hops FROM " TBL_CLASS_DEPEND " WHERE class=? AND hops<=?");
        CacheQuery& s   = sorted ? qs : qu;
        s.bind(1, c.id);
        s.bind(2, maxDepth);
        return exec_class_rank_vector(s);
    }

    ClassCountMap               base_classes_ranked_merged_map(const ClassSet&cset, bool inSet)
    {
        ClassCountMap   ret;
        for(Class c : cset){
            if(inSet)
                ret[c]  = { 0 };
            for(Class::Rank cr : base_classes_ranked(c))
                ret[cr.cls].set_smaller(HCountU64{(uint64_t) cr.rank});
        }
        return ret;
    }

    ClassHopMap                 base_hops(Class c)
    {
        static thread_local CacheQuery  s("SELECT base,hops FROM " TBL_CLASS_DEPEND " WHERE class=?");
        ClassHopMap ret;
        auto af = s.af();
        s.bind(1, c.id);
        while(s.step() == SQResult::Row){
            ret[Class(s.v_uint64(1))] = { (hop_t) s.v_int(2) };
        }
        return ret;
    }

    std::string  binding(Class c)
    {
        static thread_local CacheQuery  s("SELECT binding FROM " TBL_CLASSES " WHERE id=?");
        return s.str(c.id);
    }

    std::string  brief(Class c)
    {
        static thread_local CacheQuery    s("SELECT brief FROM " TBL_CLASSES " WHERE id=?");
        return s.str(c.id);
    }
    
    Category  category(Class c)
    {
        static thread_local CacheQuery s("SELECT category FROM " TBL_CLASSES " WHERE id=?");
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
        static thread_local CacheQuery s("SELECT id FROM " TBL_CLASSES " WHERE k=?");
        return s.as<Class>(k);
    }

    Class::SharedFile  class_doc(Fragment f, cdb_options_t opts)
    {
        if(!f)
            return Class::SharedFile();
            
        std::filesystem::path       fp  = path(f);
        Id::Lock  lk;
        if(!(opts & DONT_LOCK)){
            lk  = Id(f).lock(false);
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
        if(td->load(std::move(ch), fp) != std::error_code()){
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

    std::set<Class>     classes_set(const string_set_t&sset, bool noisy)
    {
        std::set<Class>   ret;
        for(const std::string& s : sset){
            if(s.empty())
                continue;
                
            Class cls = class_(s);
            if(!cls){
                if(noisy)
                    yWarning() << "Unable to find class: " << s;
                continue;
            }
            
            ret.insert(cls);
        }
        return ret;
    }
    
    std::set<Class>     classes_set(const string_view_set_t&sset, bool noisy)
    {
        std::set<Class>   ret;
        for(std::string_view s : sset){
            if(s.empty())
                continue;
                
            Class cls = class_(s);
            if(!cls){
                if(noisy)
                    yWarning() << "Unable to find class: " << s;
                continue;
            }
            
            ret.insert(cls);
        }
        return ret;
    }
    
    std::vector<Class>          classes_with_tag(Tag x, Sorted sorted)
    {
        static thread_local CacheQuery qs("SELECT class FROM " TBL_CLASS_TAG " INNER JOIN " TBL_CLASSES " ON " TBL_CLASS_TAG ".class=" TBL_CLASSES ".id WHERE tag=? ORDER BY " TBL_CLASSES ".k");
        static thread_local CacheQuery qu("SELECT class FROM " TBL_CLASS_TAG " WHERE tag=?");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Class>(x.id);
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
    
        static thread_local CacheQuery    i("INSERT OR FAIL INTO " TBL_CLASSES " (id,k) VALUES (?,?)");
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
        ret.reserve(all.size());
        for(std::string_view  s: all)
            ret.push_back(db_class(s));
        return ret;
    }

    std::vector<Class>             db_classes(const string_view_set_t& all)
    {
        std::vector<Class>   ret;
        ret.reserve(all.size());
        for(std::string_view  s: all)
            ret.push_back(db_class(s));
        return ret;
    }

    string_set_t                def_alias_set(Class c)
    {
        static thread_local CacheQuery s("SELECT alias FROM " TBL_CLSDEF_ALIAS " WHERE class=?");
        return s.sset(c.id);
    }
    
    
    string_set_t                def_prefix_set(Class c)
    {
        static thread_local CacheQuery s("SELECT prefix FROM " TBL_CLSDEF_PREFIX " WHERE class=?");
        return s.sset(c.id);
    }

    std::set<Class>             def_reverse_set(Class c)
    {
        static thread_local CacheQuery  s("SELECT reverse FROM " TBL_CLSDEF_REVERSE " WHERE class=?");
        return s.set<Class>(c.id);
    }
    
    std::set<Class>             def_rev_reverse_set(Class c)
    {
        static thread_local CacheQuery  s("SELECT class FROM " TBL_CLSDEF_REVERSE " WHERE reverse=?");
        return s.set<Class>(c.id);
    }

    std::vector<Class>          def_rev_use(Class c, Sorted sorted)
    {
        static thread_local CacheQuery qs("SELECT use FROM " TBL_CLSDEF_USE " INNER JOIN " TBL_CLASSES " ON " TBL_CLSDEF_USE ".class=" TBL_CLASSES ".id WHERE use=? ORDER BY " TBL_CLASSES ".K");
        static thread_local CacheQuery qu("SELECT use FROM " TBL_CLSDEF_USE " WHERE use=?");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Class>(c.id);
    }

    std::set<Class>             def_rev_use_set(Class c)
    {
        static thread_local CacheQuery  s("SELECT class FROM " TBL_CLSDEF_USE " WHERE use=?");
        return s.set<Class>(c.id);
    }
    
    
    size_t                      def_rev_use_count(Class c)
    {
        static thread_local CacheQuery s("SELECT COUNT(1) FROM " TBL_CLSDEF_USE " WHERE use=?");
        return s.size(c.id);
    }

    std::set<Class>             def_source_set(Class c)
    {
        static thread_local CacheQuery  s("SELECT source FROM " TBL_CLSDEF_SOURCE " WHERE class=?");
        return s.set<Class>(c.id);
    }

    string_set_t                def_suffix_set(Class c)
    {
        static thread_local CacheQuery s("SELECT suffix FROM " TBL_CLSDEF_SUFFIX " WHERE class=?");
        return s.sset(c.id);
    }

    std::set<Class>             def_target_set(Class c)
    {
        static thread_local CacheQuery  s("SELECT target FROM " TBL_CLSDEF_TARGET " WHERE class=?");
        return s.set<Class>(c.id);
    }

    std::set<Class>             def_use_set(Class c)
    {
        static thread_local CacheQuery  s("SELECT use FROM " TBL_CLSDEF_USE " WHERE class=?");
        return s.set<Class>(c.id);
    }

    std::vector<Class>          def_use(Class c, Sorted sorted)
    {
        static thread_local CacheQuery qs("SELECT use FROM " TBL_CLSDEF_USE " INNER JOIN " TBL_CLASSES " ON " TBL_CLSDEF_USE ".use=" TBL_CLASSES ".id WHERE class=? ORDER BY " TBL_CLASSES ".K");
        static thread_local CacheQuery qu("SELECT use FROM " TBL_CLSDEF_USE " WHERE class=?");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Class>(c.id);
    }
    
    size_t                      def_use_count(Class c)
    {
        static thread_local CacheQuery s("SELECT COUNT(1) FROM " TBL_CLSDEF_USE " WHERE class=?");
        return s.size(c.id);
    }
    
    // disabled until graphs are working
    //Graph               dep_graph(Class c)
    //{   
        //static thread_local CacheQuery    s("SELECT deps FROM " TBL_CLASSES " WHERE id=?");
        //return s.as<Graph>(c.id);
    //}
    
    std::vector<Class>       derived_classes(Class c, Sorted sorted)
    {
        static thread_local CacheQuery    qs("SELECT class FROM " TBL_CLASS_DEPEND " INNER JOIN " TBL_CLASSES " ON " TBL_CLASS_DEPEND ".class=" TBL_CLASSES ".id WHERE base=? ORDER BY " TBL_CLASSES ".k");
        static thread_local CacheQuery    qu("SELECT class FROM " TBL_CLASS_DEPEND " WHERE base=?");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Class>(c.id);
    }
    
    std::vector<Class::Rank>    derived_classes_ranked(Class c, Sorted sorted)
    {
        static thread_local CacheQuery  qs("SELECT class, hops FROM " TBL_CLASS_DEPEND " INNER JOIN " TBL_CLASSES " ON " TBL_CLASS_DEPEND ".class=" TBL_CLASSES ".id WHERE base=? ORDER BY hops, " TBL_CLASSES ".k");
        static thread_local CacheQuery  qu("SELECT class, hops FROM " TBL_CLASS_DEPEND " WHERE base=?");
        CacheQuery& s = sorted ? qs : qu;
        s.bind(1, c.id);
        return exec_class_rank_vector(s);
    }
    
    std::vector<Class::Rank>    derived_classes_limited_ranked(Class c, uint64_t maxDepth, Sorted sorted)
    {
        static thread_local CacheQuery qs("SELECT class, hops FROM " TBL_CLASS_DEPEND " INNER JOIN " TBL_CLASSES " ON " TBL_CLASS_DEPEND ".class=" TBL_CLASSES ".id WHERE base=? AND hops<=? ORDER BY hops, " TBL_CLASSES ".k");
        static thread_local CacheQuery qu("SELECT class, hops FROM " TBL_CLASS_DEPEND " WHERE base=? AND hops<=?");
        
        CacheQuery& s = sorted ? qs : qu;
        s.bind(1, c.id);
        s.bind(2, maxDepth);
        return exec_class_rank_vector(s);
    }

    ClassCountMap               derived_classes_ranked_merged_map(const ClassSet&cset, bool inSet)
    {
        ClassCountMap   ret;
        for(Class c : cset){
            if(inSet)
                ret[c]  = { 0 };
            for(Class::Rank cr : derived_classes_ranked(c))
                ret[cr.cls].set_smaller(HCountU64{(uint64_t) cr.rank});
        }
        return ret;
    }

    ClassHopMap                 derived_hops(Class c)
    {
        static thread_local CacheQuery  s("SELECT class,hops FROM " TBL_CLASS_DEPEND " WHERE base=?");
        ClassHopMap ret;
        auto af = s.af();
        s.bind(1, c.id);
        while(s.step() == SQResult::Row){
            ret[Class(s.v_uint64(1))] = { (hop_t) s.v_int(2) };
        }
        return ret;
    }


    Folder              detail_folder(Class c)
    {
        return db_folder( classes_folder(), key(c));
    }

    Document            document(Class c)
    {
        return exists(c) ? Document{c.id}  : Document{};
    }
    

    
    std::vector<Class>      edge_classes_in(Class c, Sorted sorted)
    {
        static thread_local CacheQuery qs("SELECT class FROM " TBL_CLASS_TARGET " INNER JOIN " TBL_CLASSES " ON " TBL_CLASS_TARGET ".class=" TBL_CLASSES ".id WHERE target=? ORDER BY " TBL_CLASSES ".K");
        static thread_local CacheQuery qu("SELECT class FROM " TBL_CLASS_TARGET " WHERE target=?");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Class>(c.id);
    }
    
    std::vector<Class>      edge_classes_out(Class c, Sorted sorted)
    {
        static thread_local CacheQuery qs("SELECT class FROM " TBL_CLASS_SOURCE " INNER JOIN " TBL_CLASSES " ON " TBL_CLASS_SOURCE ".class=" TBL_CLASSES ".id WHERE source=? ORDER BY " TBL_CLASSES ".K");
        static thread_local CacheQuery qu("SELECT class FROM " TBL_CLASS_SOURCE " WHERE source=?");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Class>(c.id);
    }


    bool                exists(Class c)
    {
        return exists_class(c.id);
    }
    
    

    bool                exists_class(uint64_t i)
    {
        static thread_local CacheQuery s("SELECT 1 FROM " TBL_CLASSES " WHERE id=?");
        return s.present(i);
    }
    
    Field               field(Class c, std::string_view k)
    {
        static thread_local CacheQuery s("SELECT id FROM " TBL_FIELDS " WHERE class=? AND k=?");
        return s.as<Field>(c.id, k);
    }
    
    std::vector<Field>       fields(Class c, Sorted sorted)
    {
        static thread_local CacheQuery  qs("SELECT field FROM " TBL_CLASS_FIELD " INNER JOIN " TBL_FIELDS " ON " TBL_CLASS_FIELD ".field=" TBL_FIELDS ".id WHERE " TBL_CLASS_FIELD ".class=? ORDER BY " TBL_FIELDS ".K");
        static thread_local CacheQuery  qu("SELECT field FROM " TBL_CLASS_FIELD " WHERE class=?");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Field>(c.id);
    }
    
    size_t          fields_count(Class c)
    {
        static thread_local CacheQuery    s("SELECT COUNT(1) FROM " TBL_CLASS_FIELD " WHERE class=?");
        return s.size(c.id);
    }
    
    
    Image               icon(Class c)
    {
        static thread_local CacheQuery    s("SELECT icon FROM " TBL_CLASSES " WHERE id=? LIMIT 1");
        return s.as<Image>(c.id);
    }
    
    std::vector<Class>           inbound_classes(Class c, Sorted sorted)
    {
        static thread_local CacheQuery qs("SELECT class FROM " TBL_CLASS_TARGET " INNER JOIN " TBL_CLASSES " ON " TBL_CLASS_TARGET ".class=" TBL_CLASSES ".id WHERE target=? ORDER BY " TBL_CLASSES ".K");
        static thread_local CacheQuery qu("SELECT class FROM " TBL_CLASS_TARGET " WHERE target=?");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Class>(c.id);
    }
    
    std::vector<Class::Rank>      inbound_classes_ranked(Class c, Sorted sorted)
    {
        static thread_local CacheQuery qs("SELECT class,hops FROM " TBL_CLASS_TARGET " INNER JOIN " TBL_CLASSES " ON " TBL_CLASS_TARGET ".class=" TBL_CLASSES ".id WHERE target=? ORDER BY hops," TBL_CLASSES ".K");
        static thread_local CacheQuery qu("SELECT class,hops FROM " TBL_CLASS_TARGET " WHERE target=?");
        CacheQuery& s = sorted ? qs : qu;
        s.bind(1, c.id);
        return exec_class_rank_vector(s);
    }

    Class::Info         info(Class c, bool autoKey)
    {
        Class::Info    ret;
        static thread_local CacheQuery    s("SELECT k, edge, name, plural, brief, icon, deps, category, binding FROM " TBL_CLASSES " WHERE id=?");
        auto s_af = s.af();
        s.bind(1, c.id);
        if(s.step() == SQResult::Row){
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
        static thread_local CacheQuery    s("SELECT 1 FROM " TBL_CLASS_DEPEND " WHERE class=? AND base=?");
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

    bool                is_edge(Class c)
    {
        static thread_local CacheQuery    s("SELECT edge FROM " TBL_CLASSES " WHERE id=?");
        return s.boolean(c.id);
    }
    
    std::string             key(Class c)
    {
        static thread_local CacheQuery    s("SELECT k FROM " TBL_CLASSES " WHERE id=? LIMIT 1");
        return s.str(c.id);
    }
    
    
    std::string             label(Class c)
    {
        std::string s   = name(c);
        if(s.empty())
            s   = key(c);
        return s;

        //static thread_local CacheQuery    s("SELECT ifnull(name,k) FROM " TBL_CLASSES " WHERE id=?");
        //return s.str(c.id);
    }


    Class                make_class(std::string_view k, const RootDir* rt, cdb_options_t opts, bool *wasCreated)
    {
        if(k.empty()){
            yError() << "Cannot create a BLANK class.";
            return Class();
        }
        if(wasCreated)
            *wasCreated = false;
        if(!rt)
            rt      = wksp::root_first(DataRole::Config);
        if(!rt){
            yError() << "No root_dir specified to create the class in!";
            return Class{};
        }
        
        std::string cfn = class_filename(k);
        Document        doc = db_document(classes_folder(), cfn);
        bool            was = false;
        Class           c   = db_class(doc, &was);
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
        Id::Lock   lk;
        if(!(opts & DONT_LOCK))
            lk  = Id(c).lock(true);

        Class::SharedFile    td  = writable(c, rt, DONT_LOCK);
        td -> name      = k;
        td -> plural    = pluralize(td->name);
        td -> save();
        return c;
    }
    
    std::map<Class,HCountU64>   make_count_map(const std::vector<Class::Rank>& data)
    {
        std::map<Class,HCountU64>   ret;
        for(const  Class::Rank& cr : data)
            ret[cr.cls]     = HCountU64{(uint64_t) cr.rank};
        return ret;
    }

    Class::SharedData            merged(Class c, cdb_options_t opts)
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
        static thread_local CacheQuery    s("SELECT name FROM " TBL_CLASSES " WHERE id=?");
        return s.str(c.id);
    }

    NKI                 nki(Class c, bool autoKey)
    {
        static thread_local CacheQuery    s("SELECT name,icon,k FROM " TBL_CLASSES " WHERE id=?");
        auto s_af = s.af();
        s.bind(1, c.id);
        if(s.step() == SQResult::Row){
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
    
    
    std::vector<Class>           outbound_classes(Class c, Sorted sorted)
    {
        static thread_local CacheQuery qs("SELECT class FROM " TBL_CLASS_SOURCE " INNER JOIN " TBL_CLASSES " ON " TBL_CLASS_SOURCE ".class=" TBL_CLASSES ".id WHERE source=? ORDER BY " TBL_CLASSES ".K");
        static thread_local CacheQuery qu("SELECT class FROM " TBL_CLASS_SOURCE " WHERE source=?");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Class>(c.id);
    }

    std::vector<Class::Rank>     outbound_classes_ranked(Class c, Sorted sorted)
    {
        static thread_local CacheQuery qs("SELECT class,hops FROM " TBL_CLASS_SOURCE " INNER JOIN " TBL_CLASSES " ON " TBL_CLASS_SOURCE ".class=" TBL_CLASSES ".id WHERE source=? ORDER BY hops," TBL_CLASSES ".K");
        static thread_local CacheQuery qu("SELECT class,hops FROM " TBL_CLASS_SOURCE " WHERE source=?");
        CacheQuery& s = sorted ? qs : qu;
        s.bind(1, c.id);
        return exec_class_rank_vector(s);
    }
    
    std::string             plural(Class c)
    {
        static thread_local CacheQuery    s("SELECT plural FROM " TBL_CLASSES " WHERE id=?");
        return s.str(c.id);
    }

    Class::SharedFile        read(Class c, const RootDir*rt, cdb_options_t opts)
    {
        return class_doc(fragment(document(c), rt), opts);
    }

    std::vector<ClassFragDoc>    reads(Class c, cdb_options_t opts)
    {
        std::vector<ClassFragDoc>  ret;
        for(Fragment f : fragments(document(c), DataRole::Config)){
            Class::SharedFile    p   = class_doc(f, opts);
            if(p)
                ret.push_back(ClassFragDoc(f, p));
        }
        return ret;
    }

    std::vector<ClassFragDoc>  reads(Class c, class RootDir*rt, cdb_options_t opts)
    {
        std::vector<ClassFragDoc>  ret;
        for(Fragment f : fragments(document(c), rt)){
            Class::SharedFile    p   = class_doc(f, opts);
            if(p)
                ret.push_back(ClassFragDoc(f, p));
        }
        return ret;
    }

    
    std::vector<Class>       reverse_classes(Class c, Sorted sorted)
    {
        static thread_local CacheQuery    qs("SELECT reverse FROM " TBL_CLASS_REVERSE " INNER JOIN " TBL_CLASSES " ON " TBL_CLASS_REVERSE ".reverse=" TBL_CLASSES ".id WHERE class=? ORDER BY " TBL_CLASSES ".K");
        static thread_local CacheQuery    qu("SELECT reverse FROM " TBL_CLASS_REVERSE " WHERE class=?");
        CacheQuery& s   = sorted ? qs : qu;
        return s.vec<Class>(c.id);
    }
    
    size_t  reverse_classes_count(Class c)
    {
        static thread_local CacheQuery    s("SELECT COUNT(1) FROM " TBL_CLASS_REVERSE " WHERE class=?");
        return s.size(c.id);
    }

    std::vector<Class::Rank>    reverse_classes_ranked(Class c, Sorted sorted)
    {
        static thread_local CacheQuery  qs("SELECT reverse, hops FROM " TBL_CLASS_REVERSE " INNER JOIN " TBL_CLASSES " ON " TBL_CLASS_REVERSE ".reverse=" TBL_CLASSES ".id WHERE class=? ORDER BY hops, " TBL_CLASSES ".k");
        static thread_local CacheQuery  qu("SELECT reverse, hops FROM " TBL_CLASS_REVERSE " WHERE class=?");
        CacheQuery&     s   = sorted ? qs : qu;
        s.bind(1, c.id);
        return exec_class_rank_vector(s);
    }

    std::vector<Class::Rank>    reverse_classes_ranked_limited(Class c, uint64_t maxDepth, Sorted sorted)
    {
        static thread_local CacheQuery  qs("SELECT reverse, hops FROM " TBL_CLASS_REVERSE " INNER JOIN " TBL_CLASSES " ON " TBL_CLASS_REVERSE ".reverse=" TBL_CLASSES ".id WHERE class=? AND hops<=? ORDER BY hops, " TBL_CLASSES ".k");
        static thread_local CacheQuery  qu("SELECT reverse, hops FROM " TBL_CLASS_REVERSE " WHERE class=? AND hops<=?");
        CacheQuery&     s   = sorted ? qs : qu;
        s.bind(1, c.id);
        s.bind(2, maxDepth);
        return exec_class_rank_vector(s);
    }


    std::vector<Class>  sources(Class c, Sorted sorted)
    {
        static thread_local CacheQuery    qs("SELECT source FROM " TBL_CLASS_SOURCE " INNER JOIN " TBL_CLASSES " ON " TBL_CLASS_SOURCE ".source=" TBL_CLASSES ".id WHERE class=? ORDER BY " TBL_CLASSES ".K");
        static thread_local CacheQuery    qu("SELECT source FROM " TBL_CLASS_SOURCE " WHERE class=?");

        CacheQuery& s   = sorted ? qs : qu;
        return s.vec<Class>(c.id);
    }

    size_t  sources_count(Class c)
    {
        static thread_local CacheQuery    s("SELECT COUNT(1) FROM " TBL_CLASS_SOURCE " WHERE class=?");
        return s.size(c.id);
    }
    
    std::vector<Class::Rank>    source_classes_ranked(Class c, Sorted sorted)
    {
        static thread_local CacheQuery  qs("SELECT source, hops FROM " TBL_CLASS_SOURCE " INNER JOIN " TBL_CLASSES " ON " TBL_CLASS_SOURCE ".source=" TBL_CLASSES ".id WHERE class=? ORDER BY hops, " TBL_CLASSES ".k");
        static thread_local CacheQuery  qu("SELECT source, hops FROM " TBL_CLASS_SOURCE " WHERE class=?");
        CacheQuery&     s   = sorted ? qs : qu;
        s.bind(1, c.id);
        return exec_class_rank_vector(s);
    }

    std::vector<Class::Rank>    source_classes_ranked_limited(Class c, uint64_t maxDepth, Sorted sorted)
    {
        static thread_local CacheQuery  qs("SELECT source, hops FROM " TBL_CLASS_SOURCE " INNER JOIN " TBL_CLASSES " ON " TBL_CLASS_SOURCE ".source=" TBL_CLASSES ".id WHERE class=? AND hops<=? ORDER BY hops, " TBL_CLASSES ".k");
        static thread_local CacheQuery  qu("SELECT source, hops FROM " TBL_CLASS_SOURCE " WHERE class=? AND hops<=?");
        CacheQuery&     s   = sorted ? qs : qu;
        s.bind(1, c.id);
        s.bind(2, maxDepth);
        return exec_class_rank_vector(s);
    }

    TagSet               tag_set(Class c)
    {
        static thread_local CacheQuery s("SELECT tag FROM " TBL_CLASS_TAG " WHERE class=?");
        return s.set<Tag>(c.id);
    }

    bool  tagged(Class c, Tag t)
    {
        static thread_local CacheQuery s("SELECT 1 FROM " TBL_CLASS_TAG " WHERE class=? AND tag=?");
        return s.present(c.id, t.id);
    }

    TagVector  tags(Class c, Sorted sorted)
    {
        static thread_local CacheQuery qs("SELECT tag FROM " TBL_CLASS_TAG " INNER JOIN " TBL_TAGS " ON " TBL_CLASS_TAG ".tag=" TBL_TAGS ".id WHERE class=? ORDER BY " TBL_TAGS ".K");
        static thread_local CacheQuery qu("SELECT tag FROM " TBL_CLASS_TAG " WHERE class=?");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Tag>(c.id);
    }
    
    
    size_t  tags_count(Class c)
    {
        static thread_local CacheQuery s("SELECT COUNT(1) FROM " TBL_CLASS_TAG " WHERE class=?");
        return s.size(c.id);
    }

    std::vector<Class>       target_classes(Class c, Sorted sorted)
    {
        static thread_local CacheQuery  qs("SELECT target FROM " TBL_CLASS_TARGET " INNER JOIN " TBL_CLASSES " ON " TBL_CLASS_TARGET ".target=" TBL_CLASSES ".id WHERE class=? ORDER BY " TBL_CLASSES ".k");
        static thread_local CacheQuery  qu("SELECT target FROM " TBL_CLASS_TARGET " WHERE class=?");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Class>(c.id);
    }

    size_t          target_classes_count(Class c)
    {
        static thread_local CacheQuery    s("SELECT COUNT(1) FROM " TBL_CLASS_TARGET " WHERE class=?");
        return s.size(c.id);
    }

    std::vector<Class::Rank>    target_classes_ranked(Class c, Sorted sorted)
    {
        static thread_local CacheQuery  qs("SELECT target, hops FROM " TBL_CLASS_TARGET " INNER JOIN " TBL_CLASSES " ON " TBL_CLASS_TARGET ".target=" TBL_CLASSES ".id WHERE class=? ORDER BY hops, " TBL_CLASSES ".k");
        static thread_local CacheQuery  qu("SELECT target, hops FROM " TBL_CLASS_TARGET " WHERE class=?");
        CacheQuery&     s   = sorted ? qs : qu;
        s.bind(1, c.id);
        return exec_class_rank_vector(s);
    }

    std::vector<Class::Rank>    target_classes_ranked_limited(Class c, uint64_t maxDepth, Sorted sorted)
    {
        static thread_local CacheQuery  qs("SELECT target, hops FROM " TBL_CLASS_TARGET " INNER JOIN " TBL_CLASSES " ON " TBL_CLASS_TARGET ".target=" TBL_CLASSES ".id WHERE class=? AND hops<=? ORDER BY hops, " TBL_CLASSES ".k");
        static thread_local CacheQuery  qu("SELECT target, hops FROM " TBL_CLASS_TARGET " WHERE class=? AND hops<=?");
        CacheQuery&     s   = sorted ? qs : qu;
        s.bind(1, c.id);
        s.bind(2, maxDepth);
        return exec_class_rank_vector(s);
    }
    
    std::string                 url(Class c)
    {
        static thread_local CacheQuery    s("SELECT url FROM " TBL_CLASSES " WHERE class=?");
        return s.str(c.id);
    }

    std::string                 url_dev(Class c)
    {
        static thread_local CacheQuery    s("SELECT devurl FROM " TBL_CLASSES " WHERE class=?");
        return s.str(c.id);
    }
    

    Class::SharedFile        writable(Class c, const RootDir* rt, cdb_options_t opts)
    {
        if(!c)
            return Class::SharedFile();
        Document    d   = document(c);
        if(!d){
            yWarning() << "write(Class '" << key(c) << "'): Has no document!";
            return Class::SharedFile();
        }
        if(rt && !rt->is_writable(DataRole::Config)){
            yWarning() << "write(Class '" << key(c) << "'): RootDir " << rt->key << " cannot be written to!";
            return Class::SharedFile();
        }

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
