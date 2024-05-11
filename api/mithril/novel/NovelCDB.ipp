////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "NovelCDB.hpp"
#include <0/basic/ByteArray.hpp>
#include <0/io/FileUtils.hpp>
#include <0/basic/TextUtils.hpp>
#include <mithril/bit/NKI.hpp>
#include <mithril/document/DocumentCDB.hpp>
#include <mithril/folder/FolderCDB.hpp>
#include <mithril/fragment/FragmentCDB.hpp>
#include <mithril/root/RootDir.hpp>
#include <mithril/image/ImageCDB.hpp>
//#include <mithril/novel/NovelFile.hpp>
#include <mithril/tag/TagCDB.hpp>
#include <mithril/wksp/CacheQuery.hpp>

namespace yq::mithril::cdb {
    namespace {
        inline std::string novel_filename(std::string_view k)
        {
            return make_filename(k, Novel::EXTENSION);
        }
    }
    
    /*
    AtomVector        all_novel_atoms(Sorted sorted)
    {
        static thread_local CacheQuery qs("SELECT id FROM " TBL_ATOMS " WHERE isNovel=1 ORDER BY k");
        static thread_local CacheQuery qu("SELECT id FROM " TBL_ATOMS " WHERE novel!=0");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Atom>();
    }
    
    size_t              all_novel_atoms_count()
    {
        static thread_local CacheQuery s("SELECT COUNT(1) FROM " TBL_ATOMS " WHERE novel!=0");
        return s.size();
    }
    */
    
    std::vector<Novel>        all_novels(Sorted sorted)
    {
        static thread_local CacheQuery qs("SELECT id FROM " TBL_NOVELS " ORDER BY k");
        static thread_local CacheQuery qu("SELECT id FROM " TBL_NOVELS "");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Novel>();
    }
    
    size_t              all_novels_count()
    {
        static thread_local CacheQuery s("SELECT COUNT(1) FROM " TBL_NOVELS "");
        return s.size();
    }
    

#if 0
    Atom                atom(Novel l)
    {
        static thread_local CacheQuery s("SELECT atom FROM " TBL_NOVELS " WHERE id=? LIMIT 1");
        return s.as<Atom>(l.id);
    }
#endif    
    
    Novel                db_novel(Document doc, bool*wasCreated)
    {
        if(wasCreated)
            *wasCreated = false;
            
        if(!doc)
            return Novel();

        if(exists_novel(doc.id))
            return Novel{ doc.id };
        
        std::string     tk  = key(doc);
        std::string     k(strip_extension(tk));
        if(k.empty()){
            yError() << "Cannot create to blank key!";
            return Novel();
        }
        
        static thread_local CacheQuery i("INSERT INTO " TBL_NOVELS " (id,k) VALUES (?,?)");
        auto i_af   = i.af();
        i.bind(1,doc.id);
        i.bind(2, k);
        if(i.exec()){
            if(wasCreated)
                *wasCreated = true;
            return Novel(doc.id);
        } else if(exists_novel(doc.id)){
            return Novel(doc.id);
        } else {
            yError() << "Unable to create or find the novel: " << k;
            return Novel();
        }
    }

#if 0
    Folder              detail_folder(Novel l)
    {
        Folder  f   = folder(Document{l.id});
        std::string sk  = skeyb(Document{l.id});
        return db_folder(f, sk+".d");
    }
#endif
    
    Document            document(Novel l)
    {
        return exists(l) ?  Document{l.id} : Document{};
    }
    

    bool                exists(Novel l)
    {
        return exists_novel(l.id);
    }
    

    bool                exists_novel(uint64_t i)
    {
        static thread_local CacheQuery s("SELECT 1 FROM " TBL_NOVELS " WHERE id=? LIMIT 1");
        return s.present(i);
    }

    
    Image               icon(Novel l)
    {
        static thread_local CacheQuery    s("SELECT icon FROM " TBL_NOVELS " WHERE id=? LIMIT 1");
        return s.as<Image>(l.id);
    }
    

    Novel::Info          info(Novel l, bool autoKey)
    {
        Novel::Info    ret;
        static thread_local CacheQuery s("SELECT k, title, icon  FROM " TBL_NOVELS " WHERE id=?");
        auto s_af = s.af();
        s.bind(1, l.id);
        if(s.step() == SQResult::Row){
            ret.doc     = Document(l.id);
            ret.key     = s.v_string(1);
            ret.title   = s.v_string(2);
            if(autoKey && ret.title.empty())
                ret.title   = ret.key;
            //ret.atom    = { s.v_uint64(3) };
            ret.icon    = { s.v_uint64(3) };
            //ret.abbr    = s.v_string(5);
            //ret.brief   = s.v_string(6);
        }
        return ret;
    }

    //bool                    is_novel(Atom);
    
    
    std::string             key(Novel l)
    {
        static thread_local CacheQuery s("SELECT k FROM " TBL_NOVELS " WHERE id=?");
        return s.str(l.id);
    }

    std::string             label(Novel l)
    {
        static thread_local CacheQuery    s("SELECT ifnull(title,k) FROM " TBL_NOVELS " WHERE id=?");
        return s.str(l.id);
    }
    
    
    Novel                novel(uint64_t i)
    {
        return exists_novel(i) ? Novel{i} : Novel();
    }

    Novel                novel(std::string_view  k)
    {
        static thread_local CacheQuery s("SELECT id FROM " TBL_NOVELS " WHERE k=?");
        return s.as<Novel>(k);
    }

    Novel                novel(Document d, bool calc)
    {
        if(!d)
            return Novel();
        if(exists_novel(d.id))
            return Novel(d.id);
        if(calc){
            std::string k   = key(folder(d)) + "/" + skeyb(d);
            return novel(k);
        }
        return Novel();
    }
    
    #if 0
    Novel                novel(Folder f)
    {
        return novel(key(f));
    }
    #endif

    Novel                    novel_by_title(std::string_view k)
    {
        static thread_local CacheQuery    s("SELECT id FROM " TBL_NOVELS " WHERE title like ? LIMIT 1");
        return s.as<Novel>(k);
    }
    
#if 0
    
    Novel::SharedFile         novel_doc(Fragment f, cdb_options_t opts)
    {
        if(!f)
            return Novel::SharedFile();

        std::filesystem::path       fp  = path(f);

        const RootDir* rt  = root_dir(f);
        if(!rt)
            yWarning() << "No root_dir for: " << fp;

        Id::Lock  lk;
        if(!(opts & DONT_LOCK)){
            lk  = Id(f).lock(false);
            if(!lk){
                yWarning() << "Unable to get read lock on fragment: " << fp;
                return Novel::SharedFile();
            }
        }

        auto    ch   = file_bytes(fp);
        lk.free();
        if(ch.empty()){
            if(opts & ALLOW_EMPTY)
                return std::make_shared<Novel::File>() ;
            return Novel::SharedFile();
        }
        
        yWarning() << "NOVEL LOADING is not yet supported.";
        return Novel::SharedFile();
        
        #if 0
        Novel::SharedFile     td  = std::make_shared<Novel::File>();
        if(td->load(std::move(ch), fp) != std::error_code()){
            yError() << "Unable to read " << fp;
            return Novel::SharedFile();
        }
        td -> set_file(fp);
        for(auto& ctx : td -> context)
            ctx.root_dir    = rt;
        return td;
        #endif
    }

    
    Novel::SharedData         merged(Novel l, cdb_options_t opts)
    {
        yWarning() << "NOVEL LOADING is not yet supported.";
        return Novel::SharedData();
        
        #if 0
    
        if(!l)
            return Novel::SharedData();
        
        Id::Lock   lk;
        if(!(opts & DONT_LOCK)){
            lk   = Id(l).lock(false);
            if(!lk){
                yWarning() << "Unable to acquire read lock on novel: " << key(l);
                return Novel::SharedData();
            }
        }

        Novel::SharedData     ret = std::make_shared<Novel::Data>();
        for(auto& i : reads(l, opts)){
            if(opts & IS_UPDATE)
                cdb::update(i.first);
            ret -> merge(*(i.second), static_cast<bool>(opts&OVERRIDE));
        }
        return ret;
        #endif
    }
#endif

    std::string             name(Novel l)
    {
        return title(l);
    }

    NKI                 nki(Novel l, bool autoKey)
    {
        static thread_local CacheQuery    s("SELECT title,icon,k FROM " TBL_NOVELS " WHERE id=?");
        auto s_af = s.af();
        s.bind(1, l.id);
        if(s.step() == SQResult::Row){
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

#if 0
    Novel::SharedFile         read(Novel l, const RootDir* rt, cdb_options_t opts)
    {
        return novel_doc(fragment(document(l), rt), opts);
    }

    std::vector<NovelFragDoc>     reads(Novel l, cdb_options_t opts)
    {
        std::vector<NovelFragDoc>  ret;
        for(Fragment f : fragments(document(l), DataRole::DB)){
            Novel::SharedFile p   = novel_doc(f, opts);
            if(p)
                ret.push_back(NovelFragDoc(f,p));
        }
        return ret;
    }

    std::vector<NovelFragDoc>   reads(Novel l, class RootDir*rt, cdb_options_t opts)
    {
        std::vector<NovelFragDoc>  ret;
        for(Fragment f : fragments(document(l), rt)){
            Novel::SharedFile p   = novel_doc(f, opts);
            if(p)
                ret.push_back(NovelFragDoc(f,p));
        }
        return ret;
    }
#endif

    bool                        tagged(Novel l, Tag t)
    {
        static thread_local CacheQuery s("SELECT 1 FROM " TBL_NOVEL_TAG " WHERE novel=? AND tag=? LIMIT 1");
        return s.present(l.id, t.id);
    }
    
    TagVector            tags(Novel l)
    {
        static thread_local CacheQuery s("SELECT tag FROM " TBL_NOVEL_TAG " WHERE novel=?");
        return s.vec<Tag>(l.id);
    }
    
    TagSet               tags_set(Novel l)
    {
        static thread_local CacheQuery s("SELECT tag FROM " TBL_NOVEL_TAG " WHERE novel=?");
        return s.set<Tag>(l.id);
    }

    std::string             title(Novel l)
    {
        static thread_local CacheQuery s("SELECT title FROM " TBL_NOVELS " WHERE id=?");
        return s.str(l.id);
    }


#if 0
    Novel::SharedFile         write(Novel l, const RootDir* rt, cdb_options_t opts)
    {
        yWarning() << "NOVEL LOADING is not yet supported.";
        return Novel::SharedFile();
        
        #if 0
        
        if(!l)
            return Novel::SharedFile();
        Document    d   = document(l);
        if(!d){
            yWarning() << "write(Novel '" << key(l) << "'): Has no document!";
            return Novel::SharedFile();
        }
        if(rt && !rt->is_writable(DataRole::DB)){
            yWarning() << "write(Novel '" << key(l) << "'): RootDir " << rt->key << " cannot be written to!";
            return Novel::SharedFile();
        }

        Fragment    f   = rt ? fragment(d, rt) : writable(d, DataRole::DB);
        if(f)
            return novel_doc(f, opts | ALLOW_EMPTY);
            
        Folder      fo  = folder(d);
        if((fo != cdb::top_folder()) && !exists(fo, rt))
            make_directory(fo, rt);
        Novel::SharedFile ptr  = std::make_shared<Novel::File>();
        ptr -> set_file( rt -> resolve(key(d)));
        ptr -> reload();
        return ptr;
        #endif
    }
#endif
}
