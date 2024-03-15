////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "TagCDB.hpp"
#include <0/basic/ByteArray.hpp>
#include <0/io/FileUtils.hpp>
#include <0/basic/Set.hpp>
#include <mithril/bit/NKI.hpp>
#include <mithril/document/DocumentCDB.hpp>
#include <mithril/folder/FolderCDB.hpp>
#include <mithril/fragment/FragmentCDB.hpp>
#include <mithril/root/RootDir.hpp>
#include <mithril/image/ImageCDB.hpp>
#include <mithril/leaf/LeafCDB.hpp>
#include <mithril/wksp/CacheQuery.hpp>
#include <mithril/wksp/Workspace.hpp>

namespace yq::mithril::cdb {
    namespace {
        inline std::string tag_filename(std::string_view k)
        {
            return make_filename(k, Tag::EXTENSION);
        }
    }

    // ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    std::vector<Tag>    all_tags(Sorted sorted)
    {
        static thread_local CacheQuery    qs("SELECT id FROM " TBL_TAGS " ORDER BY k");
        static thread_local CacheQuery    qu("SELECT id FROM " TBL_TAGS "");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Tag>();
    }
    
    size_t              count_tags()
    {
        static thread_local CacheQuery s("SELECT COUNT(1) FROM " TBL_TAGS "");
        return s.size();
    }

    bool                exists_tag(uint64_t i)
    {
        static thread_local CacheQuery s("SELECT 1 FROM " TBL_TAGS " WHERE id=? LIMIT 1");
        return s.present(i);
    }

    Tag                 find_tag(std::string_view k)
    {
        static thread_local CacheQuery    s("SELECT id FROM " TBL_TAGS " WHERE k=?");
        return s.as<Tag>(k);
    }
    
    Tag                 find_tag(uint64_t i)
    {
        return exists_tag(i) ? Tag{i} : Tag{};
    }
    
    Tag                 find_tag(Document doc, bool calc)
    {
        if(!doc)
            return Tag{};
        if(exists_tag(doc.id))
            return Tag{doc.id};
        if(calc && (folder(doc) == tags_folder())){
            std::string k   = base_key(doc);
            return find_tag(k);
        }
        return Tag{};
    }

    std::vector<Tag>             find_tags(const string_set_t&keys, bool noisy)
    {
        std::vector<Tag> ret;
        for(const std::string& s : keys){
            if(s.empty())
                continue;
                
            Tag t   = find_tag(s);
            if(!t){
                if(noisy)
                    yWarning() << "Unable to find tag: " << s;
                continue;
            }
            ret.push_back(t);
        }
        return ret;
    }

    std::vector<Tag>             find_tags(const string_view_set_t&keys, bool noisy)
    {
        std::vector<Tag> ret;
        for(const std::string_view& s : keys){
            if(s.empty())
                continue;
                
            Tag t   = find_tag(s);
            if(!t){
                if(noisy)
                    yWarning() << "Unable to find tag: " << s;
                continue;
            }
            ret.push_back(t);
        }
        return ret;
    }

    std::set<Tag>           find_tags_set(const string_set_t& keys, bool noisy)
    {
        std::set<Tag> ret;
        for(const std::string& s : keys){
            if(s.empty())
                continue;
                
            Tag t   = find_tag(s);
            if(!t){
                if(noisy)
                    yWarning() << "Unable to find tag: " << s;
                continue;
            }
            ret.insert(t);
        }
        return ret;
    }

    std::set<Tag>           find_tags_set(const string_view_set_t& keys, bool noisy)
    {
        std::set<Tag> ret;
        for(std::string_view s : keys){
            if(s.empty())
                continue;
                
            Tag t   = find_tag(s);
            if(!t){
                if(noisy)
                    yWarning() << "Unable to find tag: " << s;
                continue;
            }
            ret.insert(t);
        }
        return ret;
    }        
    
    // ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    Image   best_image(Tag x)
    {
        Document    d   = document(x);
        if(d)
            return best_image(d);
        return Image{};
    }

    std::string                 brief(Tag t)
    {
        static thread_local CacheQuery    s("SELECT brief FROM " TBL_TAGS " WHERE id=?");
        return s.str(t.id);
    }

    Document            document(Tag t)
    {
        return valid(t) ? Document{t.id} : Document{};
    }    

    Image               icon(Tag t)
    {
        static thread_local CacheQuery    s("SELECT icon FROM " TBL_TAGS " WHERE id=? LIMIT 1");
        return s.as<Image>(t.id);
    }
    
    
    Tag::Info           info(Tag t, bool autoKey)
    {
        Tag::Info    ret;
        static thread_local CacheQuery    s("SELECT brief,k,name,icon,leaf FROM " TBL_TAGS " WHERE id=?");
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
            ret.leaf        = Leaf{ s.v_uint64(5) };
        }
        return ret;
    }
    

    std::string key(Tag t)
    {
        static thread_local CacheQuery    s("SELECT k FROM " TBL_TAGS " WHERE id=?");
        return s.str(t.id);
    }
    
    std::string label(Tag t)
    {
        static thread_local CacheQuery    s("SELECT ifnull(name,k) FROM " TBL_TAGS " WHERE id=?");
        return s.str(t.id);
    }
    
    Leaf                leaf(Tag t)
    {
        static thread_local CacheQuery    s("SELECT leaf FROM " TBL_TAGS " WHERE id=?");
        return s.as<Leaf>(t.id);
    }
    
    std::string    name(Tag t)
    {
        static thread_local CacheQuery    s("SELECT name FROM " TBL_TAGS " WHERE id=?");
        return s.str(t.id);
    }
    
    
    NKI                 nki(Tag t, bool autoKey)
    {
        static thread_local CacheQuery    s("SELECT name,icon,k FROM " TBL_TAGS " WHERE id=?");
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

    bool                valid(Tag t)
    {
        return t.id && exists_tag(t.id);
    }

    // ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

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
        
        static thread_local CacheQuery    i("INSERT OR FAIL INTO " TBL_TAGS " (k,id) VALUES (?,?)");
        auto i_lk   = i.af();
        
        i.bind(1, k);
        i.bind(2, doc.id);
        if(i.step(false) == SQResult::Done){
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

    std::set<Tag>           db_tags_set(const string_set_t&all)
    {
        std::set<Tag>     ret;
        for(const std::string& s:all)
            ret.insert(db_tag(s));
        return ret;
    }

    std::set<Tag>           db_tags_set(const string_view_set_t& all)
    {
        std::set<Tag>     ret;
        for(const std::string_view& s:all)
            ret.insert(db_tag(s));
        return ret;
    }


    Tag                     make_tag(std::string_view k, const RootDir* rt, cdb_options_t opts, bool* wasCreated)
    {
        if(wasCreated)
            *wasCreated = false;
        if(k.empty()){
            yError() << "Cannot create a BLANK tag.";
            return Tag();
        }
    
        if(!rt)
            rt  = wksp::root_first(DataRole::Config);
        if(!rt){
            yError() << "No root_dir specified to create the tag in!";
            return Tag{};
        }
        
        std::string     tfn = tag_filename(k);
        Document    doc = db_document(tags_folder(), tfn);
        bool            was = false;
        Tag         t   = db_tag(doc, &was);
        if(!t){
            yWarning() << "Unable to create/find tag: " << k;
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
        Tag::SharedFile td  = write(t, rt, opts);
        td -> name  = k;
        td -> save();
        return t;
    }

    Tag::SharedData   merged(Tag t, cdb_options_t opts)
    {
        if(!t)
            return Tag::SharedData();
        
        Id::Lock   lk;
        if(!(opts & DONT_LOCK)){
            lk   = Id(t).lock(false);
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
    
    
    Tag::SharedFile          read(Tag t, const RootDir* rt, cdb_options_t opts)
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

    std::vector<TagFragDoc>    reads(Tag t, class RootDir* rt, cdb_options_t opts)
    {
        std::vector<TagFragDoc>  ret;
        for(Fragment f : fragments(document(t), rt)){
            Tag::SharedFile  p   = tag_doc(f, opts);
            if(p)
                ret.push_back(TagFragDoc(f,p));
        }
        return ret;
    }
    
    bool                set_brief(Tag t, std::string_view k, class RootDir* rt)
    {
        auto tf = write(t, rt);
        if(!tf)
            return false;
        tf -> brief     = k;
        return tf -> save() == std::error_code();
    }
    
    bool                set_name(Tag t, std::string_view k, class RootDir* rt)
    {
        auto tf = write(t, rt);
        if(!tf)
            return false;
        tf -> name     = k;
        return tf -> save() == std::error_code();
    }
    
    bool                set_notes(Tag t, std::string_view k, class RootDir* rt)
    {
        auto tf = write(t, rt);
        if(!tf)
            return false;
        tf -> notes      = k;
        return tf -> save() == std::error_code();
    }
    

    Tag                     tag(Document doc)
    {
        if(exists_tag(doc.id))
            return Tag(doc.id);
        return Tag();
    }

    Tag::SharedFile      tag_doc(Fragment f, cdb_options_t opts)
    {
        if(!f)
            return Tag::SharedFile();
            
        std::filesystem::path       fp  = path(f);
        Id::Lock  lk;
        if(!(opts & DONT_LOCK)){
            lk  = Id(f).lock(false);
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
        if(td->load(std::move(ch), fp) != std::error_code()){
            yError() << "Unable to parse tag file: " << fp;
            return Tag::SharedFile();
        }
        td -> set_file(fp);
        return td;
    }



    //void                    update(Tag t, Leaf l)
    //{
        //static thread_local CacheQuery u("UPDATE Tags SET leaf=? WHERE id=?");
        //u.exec(l.id, t.id);
    //}


    
    Tag::SharedFile         write(Tag t, const RootDir* rt, cdb_options_t opts)
    {
        if(!t)
            return Tag::SharedFile();
    
        Document    d   = document(t);
        if(!d){
            yWarning() << "write(Tag '" << key(t) << "'): Has no document!";
            return Tag::SharedFile();
        }
        if(rt && !rt->is_writable(DataRole::Config)){
            yWarning() << "write(Tag '" << key(t) << "'): RootDir " << rt->key << " cannot be written to!";
            return Tag::SharedFile();
        }
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
