////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "CategoryArg.hpp"
#include "CategoryCDB.hpp"
#include "CategoryData.hpp"
#include "CategoryFile.hpp"
#include "CategoryHtml.hpp"
#include "TagArg.hpp"
#include "TagCDB.hpp"
#include "TagData.hpp"
#include "TagFile.hpp"
#include "TagHtml.hpp"

#include <yq/atom/Class.hpp>
#include <yq/atom/Field.hpp>
#include <yq/bit/KeyValue.hpp>
#include <yq/io/Strings.hpp>
#include <yq/db/NKI.hpp>
#include <yq/db/SQ.hpp>
#include <yq/file/DocumentCDB.hpp>
#include <yq/file/DocumentHtml.hpp>
#include <yq/file/FolderCDB.hpp>
#include <yq/file/FragmentCDB.hpp>
#include <yq/file/Root.hpp>
#include <yq/image/ImageCDB.hpp>
#include <yq/image/ImageHtml.hpp>
#include <yq/io/file_utils.hpp>
#include <yq/leaf/LeafCDB.hpp>
#include <yq/text/text_utils.hpp>
#include <yq/web/WebContext.hpp>
#include <yq/web/WebHtml.hpp>
#include <yq/wksp/Workspace.hpp>

#include <yq/db/IDLock.ipp>

namespace yq {

    template class IDLock<Category>;
    template class IDLock<Tag>;

////////////////////////////////////////////////////////////////////////////////
//  CATEGORY
////////////////////////////////////////////////////////////////////////////////


    Category::Data&   Category::Data::merge(const Data&b, bool fOverride)
    {
        set_if_empty(name, b.name, fOverride);
        set_if_empty(brief, b.brief, fOverride);
        set_if_empty(notes, b.notes, fOverride);
        return *this;
    }

////////////////////////////////////////////////////////////////////////////////

    void    Category::File::reset() 
    {
        *(Data*) this = Data{};
    }

    bool    Category::File::read(KVTree&&attrs, std::string_view fname) 
    { 
        name        = attrs.value(szName);
        brief       = attrs.value(szBrief);
        notes       = attrs.value(szNotes);
        return true; 
    }
        
    bool    Category::File::write(KVTree&attrs) const 
    { 
        if(!name.empty())
            attrs.set(szName, name);
        if(!brief.empty())
            attrs.set(szBrief, brief);
        if(!notes.empty())
            attrs.set(szNotes, notes);
        return true; 
    }

////////////////////////////////////////////////////////////////////////////////

    namespace arg {
        Category category(std::string_view  arg_string)
        {
            arg_string   = trimmed(arg_string);
            if(arg_string.empty())
                return Category{};
                
            Category t   = cdb::category( arg_string);
            if(t)
                return t;
            uint64_t    i = to_uint64( arg_string).value;
            if(cdb::exists_category(i))
                return Category{i};
            return Category{};
        }
        
        Category category(const WebContext&ctx, bool *detected)
        {
            std::string    k    = ctx.find_query("id");
            if(!k.empty())
                return category_id(k);
            
            k       = ctx.find_query("key");
            if(!k.empty())
                return category_key(k);
            
            k       = ctx.find_query("cat");
            if(!k.empty())
                return category(k);
            return Category{};
        }
        
        
        Category category(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return category(arg_string);
        }
        
        Category category(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return category(arg_string);
        }

        Category category_id(std::string_view arg_string)
        {
            uint64_t    i   = to_uint64(arg_string).value;
            if(cdb::exists_category(i))
                return Category{i};
            return Category{};
        }

        Category category_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return category_id(arg_string);
        }
        
        Category category_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return category_id(arg_string);
        }
        
        Category category_key(std::string_view arg_string)
        {
            return cdb::category(trimmed(arg_string));
        }
        
        Category category_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return category_key(arg_string);
        }
        
        Category category_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return category_key(arg_string);
        }
    }

////////////////////////////////////////////////////////////////////////////////

    namespace cdb {
        namespace {
            inline std::string category_filename(std::string_view k)
            {
                return make_filename(k, Category::szExtension);
            }
            
            std::vector<Category>     all_categories_sorted()
            {
                static thread_local SQ    s("SELECT id FROM Categories ORDER BY k");
                return s.vec<Category>();
            }
            
            std::vector<Category>     all_categories_unsorted()
            {
                static thread_local SQ    s("SELECT id FROM Categories");
                return s.vec<Category>();
            }
        }

        std::vector<Category>         all_categories(Sorted sorted)
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

        std::vector<Class>  classes(Category cat)
        {
            static thread_local SQ s("SELECT id FROM Classes WHERE category=?");
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

        void                erase(Category x)
        {
            static thread_local SQ  stmts[] = {
                SQ( "UPDATE Classes SET category=0 WHERE category=?" ),
                SQ( "DELETE FROM Categories WHERE id=?" )
            };
            
            if(!x)
                return ;
            
            for(auto& sq : stmts)
                sq.exec(x.id);
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

        std::vector<Field>      fields(Category cat)
        {
            static thread_local SQ s("SELECT id FROM Fields WHERE category=?");
            return s.vec<Field>(cat.id);
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
            static thread_local SQ    s("SELECT ifnull(name,k) FROM Categories WHERE id=?");
            return s.str(t.id);
        }
        
        Category                     make_category(std::string_view k, const Root* rt, cdb_options_t opts)
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

        Category::SharedData   merged(Category t, cdb_options_t opts)
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
        
        Category::SharedFile          read(Category t, const Root* rt, cdb_options_t opts)
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

        std::vector<CatFragDoc>    reads(Category t, class Root* rt, cdb_options_t opts)
        {
            std::vector<CatFragDoc>  ret;
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
        
        void                update_icon(Category x)
        {
            if(!x)
                return ;

            static thread_local SQ u1("UPDATE Categories SET icon=? WHERE id=?");
            static thread_local SQ u2("UPDATE Documents SET icon=? WHERE id=?");

            Document    doc     = document(x);
            Image       img     = best_image(doc);
            
            if(img != icon(x)){
                u1.exec(img.id, x.id);
                u2.exec(doc.id, x.id);
            }
        }
        
        Category::SharedData           update(Category x, cdb_options_t opts)
        {
            if(!x)
                return Category::SharedData();

            if(opts & U_ICON)
                update_icon(x);
            
            Category::SharedData data = merged(x, opts);
            if(!data)
                return Category::SharedData();
                
            static thread_local SQ uInfo("UPDATE Categories SET name=?,brief=? WHERE id=?");
            
            if(opts & U_INFO){
                uInfo.bind(1, data->name);
                uInfo.bind(2, data->brief);
                uInfo.bind(3, x.id);
                uInfo.exec();
            }
            return data;
        }

        
        Category::SharedFile         write(Category t, const Root* rt, cdb_options_t opts)
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

////////////////////////////////////////////////////////////////////////////////

    namespace html {
        WebHtml&    operator<<(WebHtml& h, Category v)
        {
            Thumbnail th = cdb::thumbnail(cdb::icon(v), h.context().session.icon_size);
            
            //  start the url (later)
            if(th)
                h << th << " ";
            
            h << cdb::label(v);
            //  end the url (later)
            return h;
        }
    
        WebHtml&    operator<<(WebHtml&h, Dev<Category> v)
        {
            std::string n       = cdb::label(v.data);
            if(n.empty())
                n   = "(no-name)";
            if(v.data)
                h << "<a href=\"/dev/category?id=" << v.data.id << "\">";
            h << "[" << v.data.id << "] " << n;
            if(v.data)
                h << "</a>";
            return h;
        }

        WebHtml&    operator<<(WebHtml&h, DevID<Category> v)
        {
            if(v.data)
                h << "<a href=\"/dev/category?id=" << v.data.id << "\">";
            h << v.data.id;
            if(v.data)
                h << "</a>";
            return h;
        }

        void    dev_table(WebHtml& h, const std::vector<Category>&categorys)
        {
            auto ta = h.table();
            h << "<tr><th>ID</th><th>Key</th><th>Name</th><th>Doc</th><th>Brief</th></tr>\n";
            for(Category t : categorys){
                auto i  = cdb::info(t);
                h << "<tr><td>" << dev_id(t) << "</td><td>" << i.key << "</td><td>" << i.name
                    << "</td><td>" << dev(i.doc) << "</td><td>" << i.brief << "</td></tr>\n";
            }
        }

    }

////////////////////////////////////////////////////////////////////////////////
//  TAG
////////////////////////////////////////////////////////////////////////////////

    Tag::Data& Tag::Data::merge(const Data&b, bool fOverride)
    {
        set_if_empty(leaf, b.leaf, fOverride);
        set_if_empty(name, b.name, fOverride);
        set_if_empty(brief, b.brief, fOverride);
        set_if_empty(notes, b.notes, fOverride);
        return *this;
    }

    void    Tag::Data::reset() 
    {
        name.clear();
        leaf.clear();
        brief.clear();
        notes.clear();
    }

////////////////////////////////////////////////////////////////////////////////


    void    Tag::File::reset() 
    {
        Data::reset();
    }

    #if TAG_XML_RESAVE
    bool    Tag::File::read(const XmlDocument&doc, std::string_view fname) 
    {
        const XmlNode*  xn  = doc.first_node(szYQTag);
        if(!xn){
            yError() << "No appropriate root element!  In: '" << fname << "'";
            return false;
        }
        
        xn      = xn -> first_node(szTag);
        if(!xn){
            yError() << "No tag! In: " << fname << "'";
            return false;
        }
        
        name        = read_child(xn, szName, x_string);
        brief       = read_child(xn, szBrief, x_string);
        notes       = read_child(xn, szNotes, x_string);
        leaf        = read_child(xn, szLeaf, x_string);
        return true;
    }
    #endif

    bool    Tag::File::read(KVTree&&attrs, std::string_view fname) 
    {
        name    = attrs.value(kv::key({"%", "name"}));
        brief   = attrs.value("brief");
        notes   = attrs.value("notes");
        leaf    = attrs.value("leaf");
        return true;
    }
    
    bool    Tag::File::write(KVTree&attrs) const 
    {
        if(!name.empty())
            attrs.set("%", name);
        if(!brief.empty())
            attrs.set("brief", brief);
        if(!notes.empty())
            attrs.set("notes", notes);
        if(!leaf.empty())
            attrs.set("leaf", leaf);
        return true;
    }

////////////////////////////////////////////////////////////////////////////////


    namespace arg {
        Tag tag(std::string_view  arg_string)
        {
            arg_string   = trimmed(arg_string);
            if(arg_string.empty())
                return Tag{};
                
            Tag t   = cdb::tag( arg_string);
            if(t)
                return t;
            uint64_t    i = to_uint64( arg_string).value;
            if(cdb::exists_tag(i))
                return Tag{i};
            return Tag{};
        }
        
        Tag tag(const WebContext&ctx, bool *detected)
        {
            std::string    k    = ctx.find_query("id");
            if(!k.empty())
                return tag_id(k);
            
            k       = ctx.find_query("key");
            if(!k.empty())
                return tag_key(k);
            
            k       = ctx.find_query("tag");
            if(!k.empty())
                return tag(k);
            return Tag{};
        }
        
        
        Tag tag(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return tag(arg_string);
        }
        
        Tag tag(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return tag(arg_string);
        }

        Tag tag_id(std::string_view arg_string)
        {
            uint64_t    i   = to_uint64(arg_string).value;
            if(cdb::exists_tag(i))
                return Tag{i};
            return Tag{};
        }

        Tag tag_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return tag_id(arg_string);
        }
        
        Tag tag_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return tag_id(arg_string);
        }
        
        Tag tag_key(std::string_view arg_string)
        {
            return cdb::tag(trimmed(arg_string));
        }
        
        Tag tag_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return tag_key(arg_string);
        }
        
        Tag tag_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return tag_key(arg_string);
        }
    }

////////////////////////////////////////////////////////////////////////////////

    namespace cdb {
        namespace {
            inline std::string tag_filename(std::string_view k)
            {
                return make_filename(k, Tag::szExtension);
            }
            
            std::vector<Tag>     all_tags_sorted()
            {
                static thread_local SQ    s("SELECT id FROM Tags ORDER BY k");
                return s.vec<Tag>();
            }
            
            std::vector<Tag>     all_tags_unsorted()
            {
                static thread_local SQ    s("SELECT id FROM Tags");
                return s.vec<Tag>();
            }
        }

        std::vector<Tag>         all_tags(Sorted sorted)
        {
            return sorted ? all_tags_sorted() : all_tags_unsorted();
        }
        
        size_t                      all_tags_count()
        {
            static thread_local SQ s("SELECT COUNT(1) FROM Tags");
            return s.size();
        }

        Image   best_image(Tag x)
        {
            Document    d   = document(x);
            if(d)
                return best_image(d);
            return Image{};
        }

        std::string                 brief(Tag t)
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
            std::string k   = base_key(doc);
            if(k.empty())
                return Tag();
            
            static thread_local SQ    i("INSERT OR FAIL INTO Tags (k,id) VALUES (?,?)");
            auto i_lk   = i.af();
            
            i.bind(1, k);
            i.bind(2, doc.id);
            if(i.step(false) == SqlQuery::Done){
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

        Tag                 db_tag(std::string_view k, bool* wasCreated)
        {
            std::string     tfn = tag_filename(k);
            return db_tag(db_document(tags_folder(), tfn), wasCreated);
        }
        
        std::vector<Tag>    db_tags(const string_set_t&all)
        {
            std::vector<Tag>     ret;
            for(const std::string& s:all)
                ret.push_back(db_tag(s));
            return ret;
        }
        
        std::vector<Tag>    db_tags(const string_view_set_t&all)
        {
            std::vector<Tag>     ret;
            for(const std::string_view& s:all)
                ret.push_back(db_tag(s));
            return ret;
        }

        Document            document(Tag t)
        {
            return exists(t) ? Document{t.id} : Document{};
        }    

        void                erase(Tag x)
        {
            static thread_local SQ  stmts[] = {
                SQ( "DELETE FROM CTags WHERE tag=?" ),
                SQ( "DELETE FROM FTags WHERE tag=?" ),
                SQ( "DELETE FROM LTags WHERE tag=?" ),
                SQ( "DELETE FROM Tags WHERE id=?" )
            };
            for(auto& sq : stmts)
                sq.exec(x.id);
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
            static thread_local SQ    s("SELECT brief,k,name,icon,leaf FROM Tags WHERE id=?");
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
                ret.leaf        = Leaf{ s.v_uint64(5) };
            }
            return ret;
        }
        

        std::string key(Tag t)
        {
            static thread_local SQ    s("SELECT k FROM Tags WHERE id=?");
            return s.str(t.id);
        }
        
        std::string label(Tag t)
        {
            static thread_local SQ    s("SELECT ifnull(name,k) FROM Tags WHERE id=?");
            return s.str(t.id);
        }
        
        Leaf                leaf(Tag t)
        {
            static thread_local SQ    s("SELECT leaf FROM Tags WHERE id=?");
            return s.as<Leaf>(t.id);
        }

        Tag                     make_tag(std::string_view k, const Root* rt, cdb_options_t opts, bool* wasCreated)
        {
            if(wasCreated)
                *wasCreated = false;
        
            if(!rt)
                rt  = wksp::root_first(DataRole::Config);
            if(!rt){
                yError() << "No root specified to create the tag in!";
                return Tag{};
            }
            
            std::string     tfn = tag_filename(k);
            Document    doc = db_document(tags_folder(), tfn);
            bool            was = false;
            Tag         t   = db_tag(doc, &was);
            if(wasCreated)
                *wasCreated = was;
            if(!was)
                return t;
            if(fragments_count(doc))
                return t;
                
            Tag::Lock   lk;
            if(!(opts & DONT_LOCK))
                lk  = Tag::Lock::write(t);
            
                // prelude, we're first....
            Tag::SharedFile td  = write(t, rt, opts);
            td -> name  = k;
            td -> save();
            return t;
        }

        Tag::SharedData   merged(Tag t, cdb_options_t opts)
        {
            if(!t)
                return Tag::SharedData();
            
            Tag::Lock   lk;
            if(!(opts & DONT_LOCK)){
                lk   = Tag::Lock::read(t);
                if(!lk){
                    yWarning() << "Unable to acquire read lock on tag: " << key(t);
                    return Tag::SharedData();
                }
            }

            Tag::SharedData  ret = std::make_shared<Tag::Data>();
            for(auto& i : reads(t, opts)){
                if(opts & IS_UPDATE)
                    cdb::update(i.first);
                ret -> merge(*(i.second), static_cast<bool>(opts&OVERRIDE));
            }
            return ret;
        }
        
        std::string    name(Tag t)
        {
            static thread_local SQ    s("SELECT name FROM Tags WHERE id=?");
            return s.str(t.id);
        }
        
        
        NKI                 nki(Tag t, bool autoKey)
        {
            static thread_local SQ    s("SELECT name,icon,k FROM Tags WHERE id=?");
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
        
        Tag::SharedFile          read(Tag t, const Root* rt, cdb_options_t opts)
        {
            return tag_doc(fragment(document(t), rt), opts);
        }

        
        std::vector<TagFragDoc>      reads(Tag t, cdb_options_t opts)
        {
            std::vector<TagFragDoc>  ret;
            for(Fragment f : fragments(document(t), DataRole::Config)){
                Tag::SharedFile  p   = tag_doc(f,opts);
                if(p)
                    ret.push_back(TagFragDoc(f,p));
            }
            return ret;
        }

        std::vector<TagFragDoc>    reads(Tag t, class Root* rt, cdb_options_t opts)
        {
            std::vector<TagFragDoc>  ret;
            for(Fragment f : fragments(document(t), rt)){
                Tag::SharedFile  p   = tag_doc(f, opts);
                if(p)
                    ret.push_back(TagFragDoc(f,p));
            }
            return ret;
        }
        
        bool                set_brief(Tag t, std::string_view k, class Root* rt)
        {
            auto tf = write(t, rt);
            if(!tf)
                return false;
            tf -> brief     = k;
            return tf -> save();
        }
        
        bool                set_name(Tag t, std::string_view k, class Root* rt)
        {
            auto tf = write(t, rt);
            if(!tf)
                return false;
            tf -> name     = k;
            return tf -> save();
        }
        
        bool                set_notes(Tag t, std::string_view k, class Root* rt)
        {
            auto tf = write(t, rt);
            if(!tf)
                return false;
            tf -> notes      = k;
            return tf -> save();
        }
        
        Tag                 tag(std::string_view k)
        {
            static thread_local SQ    s("SELECT id FROM Tags WHERE k=?");
            return s.as<Tag>(k);
        }
        
        Tag                 tag(uint64_t i)
        {
            return exists_tag(i) ? Tag{i} : Tag{};
        }
        
        Tag                  tag(Document doc, bool calc)
        {
            if(!doc)
                return Tag{};
            if(exists_tag(doc.id))
                return Tag{doc.id};
            if(calc && (folder(doc) == tags_folder())){
                std::string k   = base_key(doc);
                return tag(k);
            }
            return Tag{};
        }

        Tag::SharedFile      tag_doc(Fragment f, cdb_options_t opts)
        {
            if(!f)
                return Tag::SharedFile();
                
            std::filesystem::path       fp  = path(f);
            Fragment::Lock  lk;
            if(!(opts & DONT_LOCK)){
                lk  = Fragment::Lock::read(f);
                if(!lk){
                    yWarning() << "Unable to get read lock on fragment: " << fp;
                    return Tag::SharedFile();
                }
            }

            auto ch   = file_bytes(fp);
            lk.free();
            if(ch.empty()){
                if(opts & ALLOW_EMPTY)
                    return std::make_shared<Tag::File>();
                return Tag::SharedFile();
            }
            
            Tag::SharedFile  td = std::make_shared<Tag::File>();
            if(!td->load(std::move(ch), fp)){
                yError() << "Unable to parse tag file: " << fp;
                return Tag::SharedFile();
            }
            td -> set_file(fp);
            return td;
        }

        std::vector<Tag>             tags(const string_set_t&keys, bool noisy)
        {
            std::vector<Tag> ret;
            for(const std::string& s : keys){
                if(s.empty())
                    continue;
                    
                Tag t   = tag(s);
                if(!t){
                    if(noisy)
                        yWarning() << "Unable to find tag: " << s;
                    continue;
                }
                ret.push_back(t);
            }
            return ret;
        }

        std::set<Tag>           tags_set(const string_set_t& keys, bool noisy)
        {
            std::set<Tag> ret;
            for(const std::string& s : keys){
                if(s.empty())
                    continue;
                    
                Tag t   = tag(s);
                if(!t){
                    if(noisy)
                        yWarning() << "Unable to find tag: " << s;
                    continue;
                }
                ret.insert(t);
            }
            return ret;
        }

        std::set<Tag>           tags_set(const string_view_set_t& keys, bool noisy)
        {
            std::set<Tag> ret;
            for(std::string_view s : keys){
                if(s.empty())
                    continue;
                    
                Tag t   = tag(s);
                if(!t){
                    if(noisy)
                        yWarning() << "Unable to find tag: " << s;
                    continue;
                }
                ret.insert(t);
            }
            return ret;
        }

        void                    update(Tag t, Leaf l)
        {
            static thread_local SQ u("UPDATE Tags SET leaf=? WHERE id=?");
            u.exec(l.id, t.id);
        }

        void                    update_icon(Tag x)
        {
            if(!x)
                return ;

            Document    doc     = document(x);
            Image       img     = best_image(doc);

            static thread_local SQ u1("UPDATE Tags SET icon=? WHERE id=?");
            static thread_local SQ u2("UPDATE Documents SET icon=? WHERE id=?");
            
            if(icon(x) != img){
                u1.exec(img.id, x.id);
                u2.exec(doc.id, x.id);
            }
        }

        Tag::SharedData         update(Tag x, cdb_options_t opts)
        {
            if(!x)
                return Tag::SharedData();

            if(opts & U_ICON)
                update_icon(x);

            auto data  = merged(x, opts);
            if(!data)
                return Tag::SharedData();
             
            static thread_local SQ uInfo("UPDATE Tags SET name=?,brief=? WHERE id=?");
            static thread_local SQ uLeaf("UPDATE Tags SET leaf=? WHERE id=?");
            
            if(opts&U_INFO){
                uInfo.bind(1, data->name);
                uInfo.bind(2, data->brief);
                uInfo.bind(3, x.id);
                uInfo.exec();
            }
            if(opts&U_LEAF){
                Leaf    l   = leaf(data->leaf);
                uLeaf.exec(l.id, x.id);
            }
            
            return data;
        }

        
        Tag::SharedFile         write(Tag t, const Root* rt, cdb_options_t opts)
        {
            Document    d   = document(t);
            if(!d)
                return Tag::SharedFile();
            if(rt && !rt->is_writable(DataRole::Config))
                return Tag::SharedFile();
            Fragment    f   = rt ? fragment(d, rt) : writable(d, DataRole::Config);
            if(f)
                return tag_doc(f, opts | ALLOW_EMPTY);

            Folder      fo  = folder(d);
            if((fo != cdb::top_folder()) && !exists(fo, rt))
                make_directory(fo, rt);

            Tag::SharedFile ptr  = std::make_shared<Tag::File>();
            ptr -> set_file( rt -> resolve(key(d)));
            ptr -> reload();
            return ptr;
        }    
    }

////////////////////////////////////////////////////////////////////////////////

    namespace html {
        WebHtml&    operator<<(WebHtml&h, Tag t)
        {
            Thumbnail th = cdb::thumbnail(cdb::icon(t), h.context().session.icon_size);
            
            //  start the url (later)
            if(th)
                h << th << " ";
            
            h << cdb::label(t);
            //  end the url (later)
            return h;
        }
    
        WebHtml&    operator<<(WebHtml&h, Dev<Tag> v)
        {
            h << "<a href=\"/dev/tag?id=" << v.data.id << "\">";
            h << "[" << v.data.id << "] " << cdb::label(v.data);
            h << "</a>";
            return h;
        }

        WebHtml&    operator<<(WebHtml&h, DevID<Tag> v)
        {
            if(v.data)
                h << "<a href=\"/dev/tag?id=" << v.data.id << "\">";
            h << v.data.id;
            if(v.data)
                h << "</a>";
            return h;
        }

        void    dev_table(WebHtml& h, const std::vector<Tag>&tags)
        {
            auto ta = h.table();
            h << "<tr><th>ID</th><th>Key</th><th>Name</th><th>Doc</th><th>Leaf</th><th>Brief</th></tr>\n";
            for(Tag t : tags){
                auto i  = cdb::info(t);
                h << "<tr><td>" << dev_id(t) << "</td><td>" << i.key << "</td><td>" << i.name
                    << "</td><td>" << dev(i.doc) << "</td><td>" << /* dev(cdb::leaf(t)) << */ "</td><td>" << i.brief << "</td></tr>\n";
            }
        }

    }

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
