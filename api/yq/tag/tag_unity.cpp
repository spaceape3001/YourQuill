////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "arg.hpp"
#include "cdb.hpp"
#include <yq/bit/KeyValue.hpp>
#include <yq/bit/NKI.hpp>
#include <yq/file/file_cdb.hpp>
#include <yq/log/Logging.hpp>
#include <yq/text/Utils.hpp>
#include <yq/type/ByteArray.hpp>
#include <yq/web/WebContext.hpp>
#include <yq/web/WebHtml.hpp>
#include <yq/wksp/CacheFwd.hpp>
#include <yq/wksp/CacheSQ.hpp>
#include <yq/wksp/Root.hpp>
#include <yq/wksp/Workspace.hpp>

namespace yq {

    namespace {
        std::string tag_filename(std::string_view k)
        {
            std::string ret(k);
            ret += ".tag";
            return ret;
        }
    }

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

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


    void    Tag::File::reset() 
    {
        Data::reset();
    }

    bool    Tag::File::read(ByteArray&&buffer, const std::string_view& fname) 
    {
        KVTree          attrs;
        if(!attrs.parse(buffer, nullptr, true, fname))
            return false;

        name        = attrs.value(kv::key("%", "%tag", "tag", "%name", "name" ));
        notes       = attrs.value(kv::key("%note", "note", "notes", "%notes" ));
        brief       = attrs.value(kv::key("%desc", "brief", "desc", "%brief" ));
        leaf        = attrs.value(kv::key("%leaf", "leaf"));
        return true;
    }

    bool    Tag::File::write(yq::Stream&chars) const
    {
        KVTree        attrs;
        if(!name.empty())
            attrs << KeyValue("%", name);
        if(!brief.empty())
            attrs << KeyValue("desc", brief);
        if(!leaf.empty())
            attrs << KeyValue("leaf", leaf);
        if(!notes.empty())
            attrs << KeyValue("note", notes);
        attrs.write(chars);
        return true;
    }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

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
        
        Tag tag(const WebContext&ctx)
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
        
        Tag tag(const WebHtml&h)
        {
            return tag(h.context());
        }
        
        Tag tag(const WebContext&ctx, std::string_view arg_name)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            return tag(arg_string);
        }
        
        Tag tag(const WebHtml&h, std::string_view arg_name)
        {
            return tag(h.context(), arg_name);
        }
        
        Tag tag(const WebContext& ctx, std::initializer_list<std::string_view> arg_names)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            return tag(arg_string);
        }
        
        Tag tag(const WebHtml&h, std::initializer_list<std::string_view> arg_names)
        {
            return tag(h.context(), arg_names);
        }

        Tag tag_id(std::string_view arg_string)
        {
            uint64_t    i   = to_uint64(arg_string).value;
            if(cdb::exists_tag(i))
                return Tag{i};
            return Tag{};
        }

        Tag tag_id(const WebContext&ctx, std::string_view arg_name)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            return tag_id(arg_string);
        }
        
        Tag tag_id(const WebHtml&h, std::string_view arg_name)
        {
            return tag_id(h.context(), arg_name);
        }
        
        Tag tag_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            return tag_id(arg_string);
        }
        
        Tag tag_id(const WebHtml&h, std::initializer_list<std::string_view> arg_names)
        {
            return tag_id(h.context(), arg_names);
        }
        
        Tag tag_key(std::string_view arg_string)
        {
            return cdb::tag(trimmed(arg_string));
        }
        
        Tag tag_key(const WebContext&ctx, std::string_view arg_name)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            return tag_key(arg_string);
        }
        
        Tag tag_key(const WebHtml&h, std::string_view arg_name)
        {
            return tag_key(h.context(), arg_name);
        }
        
        Tag tag_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            return tag_key(arg_string);
        }
        
        Tag tag_key(const WebHtml&h, std::initializer_list<std::string_view> arg_names)
        {
            return tag_key(h.context(), arg_names);
        }
    }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


    namespace cdb {
        namespace {
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

        void                erase(Tag t)
        {
            static thread_local SQ d("DELETE FROM Tags WHERE id=?");
            d.exec(t.id);
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
            static thread_local SQ    s("SELECT brief,k,name,icon FROM Tags WHERE id=?");
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
        

        std::string key(Tag t)
        {
            static thread_local SQ    s("SELECT k FROM Tags WHERE id=?");
            return s.str(t.id);
        }
        
        std::string label(Tag t)
        {
            std::string     n   = name(t);
            return n.empty() ? key(t) : n;
        }
        
        #if 0
        Leaf                leaf(Tag t)
        {
            static thread_local SQ    s("SELECT leaf FROM Tags WHERE id=?");
            return s.as<Leaf>(t.id);
        }
        #endif

        Tag                     make_tag(std::string_view k, const Root* rt)
        {
            if(!rt)
                rt  = wksp::root_first(DataRole::Tags);
            if(!rt){
                yError() << "No root specified to create the tag in!";
                return Tag{};
            }
            
            std::string     tfn = tag_filename(k);
            Document    doc = db_document(tags_folder(), tfn);
            bool            was = false;
            Tag         t   = db_tag(doc, &was);
            if(!was)
                return t;
            if(fragments_count(doc))
                return t;
            Tag::SharedFile td  = write(t, rt);
            td -> name  = k;
            td -> save();
            return t;
        }

        Tag::SharedData   merged(Tag t, unsigned int opts)
        {
            Tag::SharedData  ret = std::make_shared<Tag::Data>();
            for(auto& i :reads(t)){
                if(opts & IsUpdate)
                    cdb::update(i.first);
                ret -> merge(*(i.second), static_cast<bool>(opts&Override));
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
        
        Tag::SharedFile          read(Tag t, const Root* rt)
        {
            return tag_doc(fragment(document(t), rt));
        }

        
        std::vector<TagFragDoc>      reads(Tag t)
        {
            std::vector<TagFragDoc>  ret;
            for(Fragment f : fragments(document(t), DataRole::Tags)){
                Tag::SharedFile  p   = tag_doc(f);
                if(p)
                    ret.push_back(TagFragDoc(f,p));
            }
            return ret;
        }

        std::vector<TagFragDoc>    reads(Tag t, class Root* rt)
        {
            std::vector<TagFragDoc>  ret;
            for(Fragment f : fragments(document(t), rt)){
                Tag::SharedFile  p   = tag_doc(f);
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
        
        Tag::SharedFile      tag_doc(Fragment f, bool fAllowEmpty)
        {
            if(!f)
                return Tag::SharedFile();

            auto ch   = frag_bytes(f);
            if(ch.empty())
                return fAllowEmpty ? std::make_shared<Tag::File>() : Tag::SharedFile();
            
            Tag::SharedFile  td = std::make_shared<Tag::File>();
            std::filesystem::path   fp  = path(f);
            if(!td->load(std::move(ch), fp)){
                yError() << "Unable to read " << fp;
                return Tag::SharedFile();
            }
            td -> set_file(fp);
            return td;
        }

        
        Tag::SharedFile         write(Tag t, const Root* rt)
        {
            Document    d   = document(t);
            if(!d)
                return Tag::SharedFile();
            if(rt && !rt->is_writable(DataRole::Tags))
                return Tag::SharedFile();
            Fragment    f   = rt ? fragment(d, rt) : writable(d, DataRole::Tags);
            if(f)
                return tag_doc(f, true);

            Folder      fo  = folder(d);
            if((fo != cdb::top_folder()) && !exists(fo, rt))
                make_directory(fo, rt);

            Tag::SharedFile ptr  = std::make_shared<Tag::File>();
            ptr -> set_file( rt -> resolve(key(d)));
            ptr -> reload();
            return ptr;
        }    
    }
}
