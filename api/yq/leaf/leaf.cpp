////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Leaf.hpp"
#include "LeafArg.hpp"
#include "LeafCDB.hpp"
#include "LeafData.hpp"
#include "LeafFile.hpp"
#include "LeafHtml.hpp"
#include "LeafLog.hpp"
#include "LeafPost.hpp"

#include <basic/CollectionUtils.hpp>
#include <yq/db/NKI.hpp>
#include <yq/db/SQ.hpp>
#include <yq/file/DocumentCDB.hpp>
#include <yq/file/FolderCDB.hpp>
#include <yq/file/FragmentCDB.hpp>
#include <yq/file/Root.hpp>
#include <yq/image/ImageCDB.hpp>
#include <basic/FileUtils.hpp>
#include <yq/org/TagCDB.hpp>
#include <yq/stream/Ops.hpp>
#include <yq/stream/Text.hpp>
#include <yq/web/WebContext.hpp>
#include <yq/web/WebHtml.hpp>

#include <yq/db/IDLock.ipp>

namespace yq {
    template class IDLock<Leaf>;

////////////////////////////////////////////////////////////////////////////////
//  LEAF
////////////////////////////////////////////////////////////////////////////////


    Leaf::Data&   Leaf::Data::merge(const Data&b, bool fOverride)
    {
        attrs.fusion(b.attrs);
        context += b.context;
        return *this;
    }

    void            Leaf::Data::reset()
    {
        attrs       = KVTree();
        context.clear();
    }

    std::string_view      Leaf::Data::abbr() const
    {
        return attrs.value(kv::key(zAbbr));
    }

    void        Leaf::Data::abbr(const std::string_view&s)
    {
        attrs.set(zAbbr, s, true);
    }

    std::string_view      Leaf::Data::brief() const
    {
        return attrs.value(zBrief);
    }

    void        Leaf::Data::brief(const std::string_view&s)
    {
        attrs.set(zBrief, s, true);
    }

    string_view_set_t Leaf::Data::classes() const
    {
        return attrs.values_set(zClass);
    }

    void        Leaf::Data::classes(const string_view_set_t&s)
    {
        attrs.set(zClass, join(s, ','));
    }

    void        Leaf::Data::classes(const string_set_t&s)
    {
        attrs.set(zClass, join(s, ','));
    }


    std::string_view      Leaf::Data::notes() const
    {
        return attrs.value(zNote);
    }

    void        Leaf::Data::notes(const std::string_view&s)
    {
        attrs.set(zNote, s, true);
    }

    string_view_set_t   Leaf::Data::tags() const
    {
        return attrs.values_set(zTag);
    }

    void        Leaf::Data::tags(const string_set_t&s)
    {
        attrs.set(zTag, join(s, ','));
    }

    void        Leaf::Data::tags(const string_view_set_t&s)
    {
        attrs.set(zTag, join(s, ','));
    }

    std::string_view      Leaf::Data::title() const
    {
        return attrs.value(zTitle);
    }

    void        Leaf::Data::title(const std::string_view&s)
    {
        attrs.set(zTitle, s, true);
    }
    
////////////////////////////////////////////////////////////////////////////////

    void    Leaf::File::reset() 
    {
        Data::reset();
    }
    
    bool    Leaf::File::read(KVTree&&tree, std::string_view body, std::string_view fname) 
    {
        attrs               = std::move(tree);
        read_kv(context, body);
        return true;
    }
    
    bool    Leaf::File::write(Stream& pipe) const
    {
        attrs.write(pipe);
        
        if(!context.empty()){
            pipe << '\n';
            write_kv(pipe, context);
        }
        
        return true;
    }
    

////////////////////////////////////////////////////////////////////////////////

    namespace arg {
        Leaf leaf(std::string_view  arg_string)
        {
            arg_string   = trimmed(arg_string);
            if(arg_string.empty())
                return Leaf{};
                
            Leaf t   = cdb::leaf( arg_string);
            if(t)
                return t;
            t       = cdb::leaf_by_title(arg_string);
            if(t)
                return t;
                
            uint64_t    i = to_uint64( arg_string).value;
            if(cdb::exists_leaf(i))
                return Leaf{i};
            return Leaf{};
        }
        
        Leaf leaf(const WebContext&ctx, bool *detected)
        {
            std::string    k    = ctx.find_query("id");
            if(!k.empty())
                return leaf_id(k);
            
            k       = ctx.find_query("key");
            if(!k.empty())
                return leaf_key(k);
            
            k       = ctx.find_query("leaf");
            if(!k.empty())
                return leaf(k);
            return Leaf{};
        }
        
        
        Leaf leaf(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return leaf(arg_string);
        }
        
        Leaf leaf(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return leaf(arg_string);
        }

        Leaf leaf_id(std::string_view arg_string)
        {
            uint64_t    i   = to_uint64(arg_string).value;
            if(cdb::exists_leaf(i))
                return Leaf{i};
            return Leaf{};
        }

        Leaf leaf_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return leaf_id(arg_string);
        }
        
        Leaf leaf_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return leaf_id(arg_string);
        }
        
        Leaf leaf_key(std::string_view arg_string)
        {
            return cdb::leaf(trimmed(arg_string));
        }
        
        Leaf leaf_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return leaf_key(arg_string);
        }
        
        Leaf leaf_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return leaf_key(arg_string);
        }

        Leaf leaf_title(std::string_view arg_string)
        {
            return cdb::leaf_by_title(trimmed(arg_string));
        }
        
        Leaf leaf_title(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return leaf_title(arg_string);
        }
        
        Leaf leaf_title(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return leaf_title(arg_string);
        }
    }

////////////////////////////////////////////////////////////////////////////////
    
    namespace cdb {
        namespace {
            inline std::string leaf_filename(std::string_view k)
            {
                return make_filename(k, Leaf::szExtension);
            }
        }


        namespace {
            std::vector<Atom>    all_leaf_atoms_sorted()
            {
                static thread_local SQ s("SELECT id FROM Atoms WHERE isLeaf=1 ORDER BY k");
                return s.vec<Atom>();
            }
            
            std::vector<Atom>    all_leaf_atoms_unsorted()
            {
                static thread_local SQ s("SELECT id FROM Atoms WHERE leaf!=0");
                return s.vec<Atom>();
            }
        }
        
        
        std::vector<Atom>        all_leaf_atoms(Sorted sorted)
        {
            return sorted ? all_leaf_atoms_sorted() : all_leaf_atoms_unsorted();
        }
        
        size_t              all_leaf_atoms_count()
        {
            static thread_local SQ s("SELECT COUNT(1) FROM Atoms WHERE leaf!=0");
            return s.size();
        }
        
        namespace {
            std::vector<Leaf>    all_leafs_sorted()
            {
                static thread_local SQ s("SELECT id FROM Leafs ORDER BY k");
                return s.vec<Leaf>();
            }

            std::vector<Leaf>    all_leafs_unsorted()
            {
                static thread_local SQ s("SELECT id FROM Leafs");
                return s.vec<Leaf>();
            }
        }
        
        std::vector<Leaf>        all_leafs(Sorted sorted)
        {
            return sorted ? all_leafs_sorted() : all_leafs_unsorted();
        }
        
        size_t              all_leafs_count()
        {
            static thread_local SQ s("SELECT COUNT(1) FROM Leafs");
            return s.size();
        }
        

        Atom                atom(Leaf l)
        {
            static thread_local SQ s("SELECT atom FROM Leafs WHERE id=? LIMIT 1");
            return s.as<Atom>(l.id);
        }
        
        
        Leaf                db_leaf(Document doc, bool*wasCreated)
        {
            if(wasCreated)
                *wasCreated = false;
                
            if(!doc)
                return Leaf();

            if(exists_leaf(doc.id))
                return Leaf{ doc.id };
            
            std::string     tk  = key(doc);
            std::string     k(strip_extension(tk));
            if(k.empty()){
                yError() << "Cannot create to blank key!";
                return Leaf();
            }
            
            static thread_local SQ i("INSERT INTO Leafs (id,k) VALUES (?,?)");
            auto i_af   = i.af();
            i.bind(1,doc.id);
            i.bind(2, k);
            if(i.exec()){
                if(wasCreated)
                    *wasCreated = true;
                return Leaf(doc.id);
            } else if(exists_leaf(doc.id)){
                return Leaf(doc.id);
            } else {
                yError() << "Unable to create or find the leaf: " << k;
                return Leaf();
            }
        }

#if 0
        Folder              detail_folder(Leaf l)
        {
            Folder  f   = folder(Document{l.id});
            std::string sk  = skeyb(Document{l.id});
            return db_folder(f, sk+".d");
        }
#endif
        
        Document            document(Leaf l)
        {
            return exists(l) ?  Document{l.id} : Document{};
        }
        
        void                erase(Leaf x)
        {
            static thread_local SQ  stmts[] = {
                SQ( "DELETE FROM LTags WHERE leaf=?" ),
                SQ( "DELETE FROM Leafs WHERE id=?" )
            };
            for(auto& sq : stmts)
                sq.exec(x.id);
        }

        bool                exists(Leaf l)
        {
            return exists_leaf(l.id);
        }
        

        bool                exists_leaf(uint64_t i)
        {
            static thread_local SQ s("SELECT 1 FROM Leafs WHERE id=? LIMIT 1");
            return s.present(i);
        }

        
        Image               icon(Leaf l)
        {
            static thread_local SQ    s("SELECT icon FROM Leafs WHERE id=? LIMIT 1");
            return s.as<Image>(l.id);
        }
        

        Leaf::Info          info(Leaf l, bool autoKey)
        {
            Leaf::Info    ret;
            static thread_local SQ s("SELECT k, title FROM Leafs WHERE id=?");
            auto s_af = s.af();
            s.bind(1, l.id);
            if(s.step() == SqlQuery::Row){
                ret.doc     = Document(l.id);
                ret.key     = s.v_string(1);
                ret.title   = s.v_string(2);
                if(autoKey && ret.title.empty())
                    ret.title   = ret.key;
            }
            return ret;
        }

        //bool                    is_leaf(Atom);
        
        
        std::string             key(Leaf l)
        {
            static thread_local SQ s("SELECT k FROM Leafs WHERE id=?");
            return s.str(l.id);
        }

        std::string             label(Leaf l)
        {
            static thread_local SQ    s("SELECT ifnull(title,k) FROM Leafs WHERE id=?");
            return s.str(l.id);
        }
        
        
        Leaf                leaf(uint64_t i)
        {
            return exists_leaf(i) ? Leaf{i} : Leaf();
        }

        Leaf                leaf(std::string_view  k)
        {
            static thread_local SQ s("SELECT id FROM Leafs WHERE k=?");
            return s.as<Leaf>(k);
        }

        Leaf                leaf(Document d, bool calc)
        {
            if(!d)
                return Leaf();
            if(exists_leaf(d.id))
                return Leaf(d.id);
            if(calc){
                std::string k   = key(folder(d)) + "/" + skeyb(d);
                return leaf(k);
            }
            return Leaf();
        }
        
        #if 0
        Leaf                leaf(Folder f)
        {
            return leaf(key(f));
        }
        #endif

        Leaf                    leaf_by_title(std::string_view k)
        {
            static thread_local SQ    s("SELECT id FROM Leafs WHERE title like ? LIMIT 1");
            return s.as<Leaf>(k);
        }
        
        Leaf::SharedFile         leaf_doc(Fragment f, cdb_options_t opts)
        {
            if(!f)
                return Leaf::SharedFile();

            std::filesystem::path       fp  = path(f);

            const Root* rt  = root(f);
            if(!rt)
                yWarning() << "No root for: " << fp;

            Fragment::Lock  lk;
            if(!(opts & DONT_LOCK)){
                lk  = Fragment::Lock::read(f);
                if(!lk){
                    yWarning() << "Unable to get read lock on fragment: " << fp;
                    return Leaf::SharedFile();
                }
            }

            auto    ch   = file_bytes(fp);
            lk.free();
            if(ch.empty()){
                if(opts & ALLOW_EMPTY)
                    return std::make_shared<Leaf::File>() ;
                return Leaf::SharedFile();
            }
            
            Leaf::SharedFile     td  = std::make_shared<Leaf::File>();
            if(!td->load(std::move(ch), fp)){
                yError() << "Unable to read " << fp;
                return Leaf::SharedFile();
            }
            td -> set_file(fp);
            for(auto& ctx : td -> context)
                ctx.root    = rt;
            return td;
        }

        
        Leaf::SharedData         merged(Leaf l, cdb_options_t opts)
        {
            if(!l)
                return Leaf::SharedData();
            
            Leaf::Lock   lk;
            if(!(opts & DONT_LOCK)){
                lk   = Leaf::Lock::read(l);
                if(!lk){
                    yWarning() << "Unable to acquire read lock on leaf: " << key(l);
                    return Leaf::SharedData();
                }
            }

            Leaf::SharedData     ret = std::make_shared<Leaf::Data>();
            for(auto& i : reads(l, opts)){
                if(opts & IS_UPDATE)
                    cdb::update(i.first);
                ret -> merge(*(i.second), static_cast<bool>(opts&OVERRIDE));
            }
            return ret;
        }
        

        NKI                 nki(Leaf l, bool autoKey)
        {
            static thread_local SQ    s("SELECT title,icon,k FROM Leafs WHERE id=?");
            auto s_af = s.af();
            s.bind(1, l.id);
            if(s.step() == SqlQuery::Row){
                NKI  ret;
                ret.name    = s.v_string(1);
                ret.icon    = Image(s.v_uint64(2)) ;
                ret.key     = s.v_string(3);
                if(autoKey && ret.name.empty())
                    ret.name    = ret.key;
                return ret;
            }
            return NKI{};
        }

        Leaf::SharedFile         read(Leaf l, const Root* rt, cdb_options_t opts)
        {
            return leaf_doc(fragment(document(l), rt), opts);
        }

        std::vector<LeafFragDoc>     reads(Leaf l, cdb_options_t opts)
        {
            std::vector<LeafFragDoc>  ret;
            for(Fragment f : fragments(document(l), DataRole::DB)){
                Leaf::SharedFile p   = leaf_doc(f, opts);
                if(p)
                    ret.push_back(LeafFragDoc(f,p));
            }
            return ret;
        }

        std::vector<LeafFragDoc>   reads(Leaf l, class Root*rt, cdb_options_t opts)
        {
            std::vector<LeafFragDoc>  ret;
            for(Fragment f : fragments(document(l), rt)){
                Leaf::SharedFile p   = leaf_doc(f, opts);
                if(p)
                    ret.push_back(LeafFragDoc(f,p));
            }
            return ret;
        }

        bool                        tagged(Leaf l, Tag t)
        {
            static thread_local SQ s("SELECT 1 FROM LTags WHERE leaf=? AND tag=? LIMIT 1");
            return s.present(l.id, t.id);
        }
        
        std::vector<Tag>            tags(Leaf l)
        {
            static thread_local SQ s("SELECT tag FROM LTags WHERE leaf=?");
            return s.vec<Tag>(l.id);
        }
        
        std::set<Tag>               tags_set(Leaf l)
        {
            static thread_local SQ s("SELECT tag FROM LTags WHERE leaf=?");
            return s.set<Tag>(l.id);
        }

        std::string             title(Leaf l)
        {
            static thread_local SQ s("SELECT title FROM Leafs WHERE id=?");
            return s.str(l.id);
        }

        Leaf::SharedData         update(Leaf x, cdb_options_t opts)
        {
            if(!x)
                return Leaf::SharedData();
            
            if(opts & U_ICON)
                update_icon(x);
            
            auto data  = merged(x, opts|IS_UPDATE);
            if(!data)
                return Leaf::SharedData();

            static thread_local SQ dTag("DELETE FROM LTags WHERE leaf=? AND tag=?");
            static thread_local SQ iTag("INSERT INTO LTags (leaf, tag) VALUES (?,?)");
            static thread_local SQ uInfo("UPDATE Leafs SET title=?, abbr=?, brief=? WHERE id=?");

            if(opts & U_INFO){
                std::string_view title   = data->title();
                std::string_view abbr    = data->abbr();
                std::string_view brief   = data->brief();
                if(title.empty())
                    title       = data->attrs.value(kv::key({"nick", "name"}));
                
                if(title.empty()){
                    uInfo.bind(1, key(x));  // fall back (for now) 
                                        // TODO ... make this smarter (later)
                } else 
                    uInfo.bind(1, title);
                uInfo.bind(2, abbr);
                uInfo.bind(3, brief);
                uInfo.bind(4, x.id);
                uInfo.exec();
            }
            
            if(opts & U_TAGS){
                std::set<Tag>   old_tags = tags_set(x);
                std::set<Tag>   new_tags = tags_set(data->tags(), true);

                auto ch_tag = add_remove(old_tags, new_tags);
                iTag.batch(x.id, ids_for(ch_tag.added));
                dTag.batch(x.id, ids_for(ch_tag.removed));
                
            }
            
            return data;
        }

        void                update_icon(Leaf x)
        {
            if(!x)
                return ;
                
            Document    doc     = document(x);
            Image       img     = best_image(doc);
            static thread_local SQ u1("UPDATE Leafs SET icon=? WHERE id=?");
            static thread_local SQ u2("UPDATE Documents SET icon=? WHERE id=?");

            if(icon(x) != img){
                u1.exec(img.id, x.id);
                u2.exec(doc.id, x.id);
            }
        }


        Leaf::SharedFile         write(Leaf l, const Root* rt, cdb_options_t opts)
        {
            if(!l)
                return Leaf::SharedFile();
            Document    d   = document(l);
            if(!d){
                yWarning() << "write(Leaf '" << key(l) << "'): Has no document!";
                return Leaf::SharedFile();
            }
            if(rt && !rt->is_writable(DataRole::DB)){
                yWarning() << "write(Leaf '" << key(l) << "'): Root " << rt->key << " cannot be written to!";
                return Leaf::SharedFile();
            }

            Fragment    f   = rt ? fragment(d, rt) : writable(d, DataRole::DB);
            if(f)
                return leaf_doc(f, opts | ALLOW_EMPTY);
                
            Folder      fo  = folder(d);
            if((fo != cdb::top_folder()) && !exists(fo, rt))
                make_directory(fo, rt);
            Leaf::SharedFile ptr  = std::make_shared<Leaf::File>();
            ptr -> set_file( rt -> resolve(key(d)));
            ptr -> reload();
            return ptr;
        }

    }

////////////////////////////////////////////////////////////////////////////////

    namespace html {
        WebHtml&    operator<<(WebHtml& h, Leaf l)
        {
            Thumbnail th = cdb::thumbnail(cdb::icon(l), h.context().session.icon_size);
            
            // start the url (later)
            if(th)
                h << th << " ";
            h << cdb::label(l);
            //  end the url(later)
            return h;
        }

        WebHtml&    operator<<(WebHtml&h, Dev<Leaf> v)
        {
            h << "<a href=\"/dev/leaf?id=" << v.data.id << "\">";
            h << "[" << v.data.id << "] " << cdb::label(v.data);
            h << "</a>";
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, DevID<Leaf> v)
        {
            if(v.data)
                h << "<a href=\"/dev/leaf?id=" << v.data.id << "\">";
            h << v.data.id;
            if(v.data)
                h << "</a>";
            return h;
        }
    
        void        dev_table(WebHtml&h, const std::vector<Leaf>&leafs)
        {
            auto t=h.table();
            h << "<tr><th><ID></th><th>key</th><th>Title</th></tr>\n";
            for(Leaf l : leafs){
                auto    i   = cdb::info(l);
                h << "<tr><td>" << dev_id(l) << "</td><td><a href=\"/leaf?id=" << l.id << "\">"
                    << i.key << "<a></td><td>" << i.title << "</td></tr>\n";
            }
        }
    }

////////////////////////////////////////////////////////////////////////////////

    namespace post {

        Leaf leaf(WebContext&ctx, bool *detected)
        {
            ctx.decode_post();
            
            if(detected)
                *detected   = false;
                
            std::string    k    = ctx.rx_post.first("leaf");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return arg::leaf_id(k);
            }
            return Leaf();
        }
        
        Leaf leaf(WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_name));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::leaf_id(arg_string);
        }
        
        Leaf leaf(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_names));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::leaf_id(arg_string);
        }
    }

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}

