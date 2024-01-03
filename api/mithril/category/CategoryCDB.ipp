////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CategoryCDB.hpp"
#include <0/basic/ByteArray.hpp>
#include <0/io/FileUtils.hpp>
#include <mithril/class/Class.hpp>
#include <mithril/field/Field.hpp>
#include <mithril/bit/NKI.hpp>
#include <mithril/document/DocumentCDB.hpp>
#include <mithril/folder/FolderCDB.hpp>
#include <mithril/fragment/FragmentCDB.hpp>
#include <mithril/root/RootDir.hpp>
#include <mithril/image/ImageCDB.hpp>
#include <mithril/wksp/CacheQuery.hpp>
#include <mithril/wksp/Workspace.hpp>

namespace yq::mithril::cdb {
    namespace {
        inline std::string category_filename(std::string_view k)
        {
            return make_filename(k, Category::EXTENSION);
        }
    }

    std::vector<Category>         all_categories(Sorted sorted)
    {
        static thread_local CacheQuery    qs("SELECT id FROM Categories ORDER BY k");
        static thread_local CacheQuery    qu("SELECT id FROM Categories");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Category>();
    }
    
    size_t                      all_categories_count()
    {
        static thread_local CacheQuery s("SELECT COUNT(1) FROM Categories");
        return s.size();
    }

    Image   best_image(Category x)
    {
        Document    d   = document(x);
        if(d)
            return best_image(d);
        return Image{};
    }

    std::string                 brief(Category t)
    {
        static thread_local CacheQuery    s("SELECT brief FROM Categories WHERE id=?");
        return s.str(t.id);
    }

    Category                 category(std::string_view k)
    {
        static thread_local CacheQuery    s("SELECT id FROM Categories WHERE k=?");
        return s.as<Category>(k);
    }
    
    Category                 category(uint64_t i)
    {
        return exists_category(i) ? Category{i} : Category{};
    }

    Category                 category(Document doc, bool calc)
    {
        if(!doc)
            return Category();
        if(exists_category(doc.id))
            return Category(doc.id);
        if(calc && (folder(doc) == categories_folder())){
            std::string     bk  = base_key(doc);
            return category(bk);
        }
        return Category();
    }
    
    Category::SharedFile      category_doc(Fragment f, cdb_options_t opts)
    {
        if(!f)
            return Category::SharedFile();
            
        std::filesystem::path       fp  = path(f);
        
        Id::Lock  lk;
        if(!(opts & DONT_LOCK)){
            lk  = Id(f).lock(false);
            if(!lk){
                yWarning() << "Unable to get read lock on fragment: " << fp;
                return Category::SharedFile();
            }
        }

        auto ch   = file_bytes(fp);
        lk.free();
        if(ch.empty()){
            if(opts & ALLOW_EMPTY)
                return std::make_shared<Category::File>();
            return Category::SharedFile();
        }
        
        Category::SharedFile  td = std::make_shared<Category::File>();
        if(td->load(std::move(ch), fp) != std::error_code()){
            yError() << "Unable to parse category file: " << fp;
            return Category::SharedFile();
        }
        td -> set_file(fp);
        return td;
    }

    std::vector<Class>  classes(Category cat)
    {
        static thread_local CacheQuery s("SELECT id FROM Classes WHERE category=?");
        return s.vec<Class>(cat.id);
    }

    Category                 db_category(Document doc, bool* wasCreated)
    {
        if(wasCreated)
            *wasCreated = false;
        if(folder(doc) != categories_folder()){
            yError() << "Rejecting category " << key(doc) << " due to not being in the '.categories' folder";        
            return Category{};
        }
        std::string k   = base_key(doc);
        if(k.empty())
            return Category();
        
        static thread_local CacheQuery    i("INSERT OR FAIL INTO Categories (k,id) VALUES (?,?)");
        auto i_lk   = i.af();
        
        i.bind(1, k);
        i.bind(2, doc.id);
        if(i.step(false) == SQResult::Done){
            if(wasCreated)
                *wasCreated = true;
            return Category{doc.id};
        } else if(exists_category(doc.id)){
            return Category{doc.id};
        } else {
            yError() << "Unable to get the category from the database: " << k;
            return Category{};
        }
    }
    
    Category                 db_category(Fragment f, bool* wasCreated)
    {
        return db_category(document(f),wasCreated);
    }

    Category                 db_category(std::string_view k, bool* wasCreated)
    {
        std::string     tfn = category_filename(k);
        return db_category(db_document(categories_folder(), tfn), wasCreated);
    }
    
    Document            document(Category t)
    {
        return exists(t) ? Document{t.id} : Document{};
    }    

    bool                exists(Category t)
    {
        return exists_category(t.id);
    }


    bool                exists_category(uint64_t i)
    {
        static thread_local CacheQuery s("SELECT 1 FROM Categories WHERE id=? LIMIT 1");
        return s.present(i);
    }

    std::vector<Field>      fields(Category cat)
    {
        static thread_local CacheQuery s("SELECT id FROM Fields WHERE category=?");
        return s.vec<Field>(cat.id);
    }

    Image               icon(Category t)
    {
        static thread_local CacheQuery    s("SELECT icon FROM Categories WHERE id=? LIMIT 1");
        return s.as<Image>(t.id);
    }
    
    
    Category::Info           info(Category t, bool autoKey)
    {
        Category::Info    ret;
        static thread_local CacheQuery    s("SELECT brief,k,name,icon FROM Categories WHERE id=?");
        auto s_af = s.af();
        s.bind(1, t.id);
        if(s.step() == SQResult::Row){
            ret.brief       = s.v_text(1);
            ret.doc         = Document(t.id);
            ret.key         = s.v_text(2);
            ret.name        = s.v_text(3);
            if(autoKey && ret.name.empty())
                ret.name    = ret.key;
            ret.icon        = Image{ s.v_uint64(4) };
        }
        return ret;
    }
    

    std::string key(Category t)
    {
        static thread_local CacheQuery    s("SELECT k FROM Categories WHERE id=?");
        return s.str(t.id);
    }
    
    std::string label(Category t)
    {
        static thread_local CacheQuery    s("SELECT ifnull(name,k) FROM Categories WHERE id=?");
        return s.str(t.id);
    }
    
    Category                     make_category(std::string_view k, const RootDir* rt, cdb_options_t opts, bool *wasCreated)
    {
        if(wasCreated)
            *wasCreated = false;
        if(k.empty()){
            yError() << "Cannot create a BLANK category.";
            return Category();
        }
        if(!rt)
            rt  = wksp::root_first(DataRole::Config);
        if(!rt){
            yError() << "No root_dir specified to create the category in!";
            return Category{};
        }
        std::string     tfn = category_filename(k);
        Document    doc = db_document(categories_folder(), tfn);
        bool            was = false;
        Category         t   = db_category(doc, &was);
        if(!t){
            yWarning() << "Unable to create/find category: " << k;
            return t;
        }
        
        if(wasCreated)
            *wasCreated = was;
        if(!was)
            return t;
        if(fragments_count(doc))
            return t;
            
        Id::Lock   lk;
        if(!(opts & DONT_LOCK))
            lk  = Id(t).lock(true);
        
            // prelude, we're first....
        Category::SharedFile td  = write(t, rt, opts);
        if(!td){
            yWarning() << "Unable to create category file!";
            return t;
        }
            
        
        td -> name  = k;
        td -> save();
        return t;
    }

    Category::SharedData   merged(Category t, cdb_options_t opts)
    {
        if(!t)
            return Category::SharedData();
        
        Id::Lock   lk;
        if(!(opts & DONT_LOCK)){
            lk   = Id(t).lock(false);
            if(!lk){
                yWarning() << "Unable to acquire read lock on category: " << key(t);
                return Category::SharedData();
            }
        }

        Category::SharedData  ret = std::make_shared<Category::Data>();
        for(auto& i : reads(t, opts)){
            if(opts & IS_UPDATE)
                cdb::update(i.first);
            ret -> merge(*(i.second), static_cast<bool>(opts&OVERRIDE));
        }
        return ret;
    }
    
    std::string    name(Category t)
    {
        static thread_local CacheQuery    s("SELECT name FROM Categories WHERE id=?");
        return s.str(t.id);
    }
    
    
    NKI                 nki(Category t, bool autoKey)
    {
        static thread_local CacheQuery    s("SELECT name,icon,k FROM Categories WHERE id=?");
        auto s_af = s.af();
        s.bind(1, t.id);
        if(s.step() == SQResult::Row){
            NKI  ret;
            ret.name    = s.v_text(1);
            ret.icon    = Image(s.v_uint64(2)) ;
            ret.key     = s.v_text(3);
            if(autoKey && ret.name.empty())
                ret.name    = ret.key;
            return ret;
        }
        return NKI{};
    }
    
    Category::SharedFile          read(Category t, const RootDir* rt, cdb_options_t opts)
    {
        return category_doc(fragment(document(t), rt), opts);
    }

    
    std::vector<CatFragDoc>      reads(Category t, cdb_options_t opts)
    {
        std::vector<CatFragDoc>  ret;
        for(Fragment f : fragments(document(t), DataRole::Config)){
            Category::SharedFile  p   = category_doc(f,opts);
            if(p)
                ret.push_back(CatFragDoc(f,p));
        }
        return ret;
    }

    std::vector<CatFragDoc>    reads(Category t, class RootDir* rt, cdb_options_t opts)
    {
        std::vector<CatFragDoc>  ret;
        for(Fragment f : fragments(document(t), rt)){
            Category::SharedFile  p   = category_doc(f, opts);
            if(p)
                ret.push_back(CatFragDoc(f,p));
        }
        return ret;
    }
    
    bool                set_brief(Category t, std::string_view k, class RootDir* rt)
    {
        auto tf = write(t, rt);
        if(!tf)
            return false;
        tf -> brief     = k;
        return tf -> save() == std::error_code();
    }
    
    bool                set_name(Category t, std::string_view k, class RootDir* rt)
    {
        auto tf = write(t, rt);
        if(!tf)
            return false;
        tf -> name     = k;
        return tf -> save() == std::error_code();
    }
    
    
    Category::SharedFile         write(Category t, const RootDir* rt, cdb_options_t opts)
    {
        if(!t)
            return Category::SharedFile();
            
        Document    d   = document(t);
        if(!d){
            yWarning() << "write(Category '" << key(t) << "'): Has no document!";
            return Category::SharedFile();
        }
        if(rt && !rt->is_writable(DataRole::Config)){
            yWarning() << "write(Category '" << key(t) << "'): RootDir " << rt->key << " cannot be written to!";
            return Category::SharedFile();
        }
            
        Fragment    f   = rt ? fragment(d, rt) : writable(d, DataRole::Config);
        if(f)
            return category_doc(f, opts | ALLOW_EMPTY);

        Folder      fo  = folder(d);
        if((fo != cdb::top_folder()) && !exists(fo, rt))
            make_directory(fo, rt);

        Category::SharedFile ptr  = std::make_shared<Category::File>();
        ptr -> set_file( rt -> resolve(key(d)));
        ptr -> reload();
        return ptr;
    }    
}
