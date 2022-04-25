////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace yq {
    namespace cdb {
        namespace {
            inline std::string category_filename(std::string_view k)
            {
                return make_filename(k, Category::szExtension);
            }
            
            Vector<Category>     all_categories_sorted()
            {
                static thread_local SQ    s("SELECT id FROM Categories ORDER BY k");
                return s.vec<Category>();
            }
            
            Vector<Category>     all_categories_unsorted()
            {
                static thread_local SQ    s("SELECT id FROM Categories");
                return s.vec<Category>();
            }
        }

        Vector<Category>         all_categories(Sorted sorted)
        {
            return sorted ? all_categories_sorted() : all_categories_unsorted();
        }
        
        size_t                      all_categories_count()
        {
            static thread_local SQ s("SELECT COUNT(1) FROM Categories");
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
            static thread_local SQ    s("SELECT brief FROM Categories WHERE id=?");
            return s.str(t.id);
        }

        Category                 category(std::string_view k)
        {
            static thread_local SQ    s("SELECT id FROM Categories WHERE k=?");
            return s.as<Category>(k);
        }
        
        Category                 category(uint64_t i)
        {
            return exists_category(i) ? Category{i} : Category{};
        }
        
        Category::SharedFile      category_doc(Fragment f, unsigned int opts)
        {
            if(!f)
                return Category::SharedFile();
                
            std::filesystem::path       fp  = path(f);
            
            Fragment::Lock  lk;
            if(!(opts & DONT_LOCK)){
                lk  = Fragment::Lock::read(f);
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
            if(!td->load(std::move(ch), fp)){
                yError() << "Unable to parse category file: " << fp;
                return Category::SharedFile();
            }
            td -> set_file(fp);
            return td;
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
            
            static thread_local SQ    i("INSERT OR FAIL INTO Categories (k,id) VALUES (?,?)");
            auto i_lk   = i.af();
            
            i.bind(1, k);
            i.bind(2, doc.id);
            if(i.step(false) == SqlQuery::Done){
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

        void                erase(Category t)
        {
            static thread_local SQ d("DELETE FROM Categories WHERE id=?");
            d.exec(t.id);
        }

        bool                exists(Category t)
        {
            return exists_category(t.id);
        }


        bool                exists_category(uint64_t i)
        {
            static thread_local SQ s("SELECT 1 FROM Categories WHERE id=? LIMIT 1");
            return s.present(i);
        }

        Image               icon(Category t)
        {
            static thread_local SQ    s("SELECT icon FROM Categories WHERE id=? LIMIT 1");
            return s.as<Image>(t.id);
        }
        
        
        Category::Info           info(Category t, bool autoKey)
        {
            Category::Info    ret;
            static thread_local SQ    s("SELECT brief,k,name,icon FROM Categories WHERE id=?");
            auto s_af = s.af();
            s.bind(1, t.id);
            if(s.step() == SqlQuery::Row){
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
            static thread_local SQ    s("SELECT k FROM Categories WHERE id=?");
            return s.str(t.id);
        }
        
        std::string label(Category t)
        {
            std::string     n   = name(t);
            return n.empty() ? key(t) : n;
        }
        
        Category                     make_category(std::string_view k, const Root* rt, unsigned int opts)
        {
            if(!rt)
                rt  = wksp::root_first(DataRole::Config);
            if(!rt){
                yError() << "No root specified to create the category in!";
                return Category{};
            }
            
            std::string     tfn = category_filename(k);
            Document    doc = db_document(categories_folder(), tfn);
            bool            was = false;
            Category         t   = db_category(doc, &was);
            if(!was)
                return t;
            if(fragments_count(doc))
                return t;
                
            Category::Lock   lk;
            if(!(opts & DONT_LOCK))
                lk  = Category::Lock::write(t);
            
                // prelude, we're first....
            Category::SharedFile td  = write(t, rt, opts);
            td -> name  = k;
            td -> save();
            return t;
        }

        Category::SharedData   merged(Category t, unsigned int opts)
        {
            if(!t)
                return Category::SharedData();
            
            Category::Lock   lk;
            if(!(opts & DONT_LOCK)){
                lk   = Category::Lock::read(t);
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
            static thread_local SQ    s("SELECT name FROM Categories WHERE id=?");
            return s.str(t.id);
        }
        
        
        NKI                 nki(Category t, bool autoKey)
        {
            static thread_local SQ    s("SELECT name,icon,k FROM Categories WHERE id=?");
            auto s_af = s.af();
            s.bind(1, t.id);
            if(s.step() == SqlQuery::Row){
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
        
        Category::SharedFile          read(Category t, const Root* rt, unsigned int opts)
        {
            return category_doc(fragment(document(t), rt), opts);
        }

        
        Vector<CatFragDoc>      reads(Category t, unsigned int opts)
        {
            Vector<CatFragDoc>  ret;
            for(Fragment f : fragments(document(t), DataRole::Config)){
                Category::SharedFile  p   = category_doc(f,opts);
                if(p)
                    ret.push_back(CatFragDoc(f,p));
            }
            return ret;
        }

        Vector<CatFragDoc>    reads(Category t, class Root* rt, unsigned int opts)
        {
            Vector<CatFragDoc>  ret;
            for(Fragment f : fragments(document(t), rt)){
                Category::SharedFile  p   = category_doc(f, opts);
                if(p)
                    ret.push_back(CatFragDoc(f,p));
            }
            return ret;
        }
        
        bool                set_brief(Category t, std::string_view k, class Root* rt)
        {
            auto tf = write(t, rt);
            if(!tf)
                return false;
            tf -> brief     = k;
            return tf -> save();
        }
        
        bool                set_name(Category t, std::string_view k, class Root* rt)
        {
            auto tf = write(t, rt);
            if(!tf)
                return false;
            tf -> name     = k;
            return tf -> save();
        }
        

        
        Category::SharedFile         write(Category t, const Root* rt, unsigned int opts)
        {
            Document    d   = document(t);
            if(!d)
                return Category::SharedFile();
            if(rt && !rt->is_writable(DataRole::Config))
                return Category::SharedFile();
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
}
