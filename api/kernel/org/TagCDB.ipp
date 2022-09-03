////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "TagCDB.hpp"
#include <basic/ByteArray.hpp>
#include <basic/FileUtils.hpp>
#include <basic/Set.hpp>
#include <kernel/db/IDLock.hpp>
#include <kernel/db/NKI.hpp>
#include <kernel/db/SQ.hpp>
#include <kernel/file/DocumentCDB.hpp>
#include <kernel/file/FolderCDB.hpp>
#include <kernel/file/FragmentCDB.hpp>
#include <kernel/file/Root.hpp>
#include <kernel/image/ImageCDB.hpp>
#include <kernel/leaf/LeafCDB.hpp>
#include <kernel/wksp/Workspace.hpp>

namespace yq {

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
            if(k.empty()){
                yError() << "Cannot create a BLANK tag.";
                return Tag();
            }
        
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

        //void                    update(Tag t, Leaf l)
        //{
            //static thread_local SQ u("UPDATE Tags SET leaf=? WHERE id=?");
            //u.exec(l.id, t.id);
        //}


        
        Tag::SharedFile         write(Tag t, const Root* rt, cdb_options_t opts)
        {
            if(!t)
                return Tag::SharedFile();
        
            Document    d   = document(t);
            if(!d){
                yWarning() << "write(Tag '" << key(t) << "'): Has no document!";
                return Tag::SharedFile();
            }
            if(rt && !rt->is_writable(DataRole::Config)){
                yWarning() << "write(Tag '" << key(t) << "'): Root " << rt->key << " cannot be written to!";
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
}

