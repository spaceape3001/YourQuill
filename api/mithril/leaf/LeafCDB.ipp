////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "LeafCDB.hpp"
#include <0/basic/ByteArray.hpp>
#include <0/io/FileUtils.hpp>
#include <0/basic/TextUtils.hpp>
#include <mithril/bit/NKI.hpp>
#include <mithril/document/DocumentCDB.hpp>
#include <mithril/folder/FolderCDB.hpp>
#include <mithril/fragment/FragmentCDB.hpp>
#include <mithril/root/RootDir.hpp>
#include <mithril/image/ImageCDB.hpp>
#include <mithril/leaf/LeafFile.hpp>
#include <mithril/tag/TagCDB.hpp>
#include <mithril/wksp/CacheQuery.hpp>

namespace yq::mithril::cdb {
    namespace {
        inline std::string leaf_filename(std::string_view k)
        {
            return make_filename(k, Leaf::EXTENSION);
        }
    }
    
    std::vector<Atom>        all_leaf_atoms(Sorted sorted)
    {
        static thread_local CacheQuery qs("SELECT id FROM " TBL_ATOMS " WHERE isLeaf=1 ORDER BY k");
        static thread_local CacheQuery qu("SELECT id FROM " TBL_ATOMS " WHERE leaf!=0");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Atom>();
    }
    
    size_t              all_leaf_atoms_count()
    {
        static thread_local CacheQuery s("SELECT COUNT(1) FROM " TBL_ATOMS " WHERE leaf!=0");
        return s.size();
    }
    
    namespace {
        std::vector<Leaf>    all_leafs_sorted()
        {
            static thread_local CacheQuery s("SELECT id FROM " TBL_LEAFS " ORDER BY k");
            return s.vec<Leaf>();
        }

        std::vector<Leaf>    all_leafs_unsorted()
        {
            static thread_local CacheQuery s("SELECT id FROM " TBL_LEAFS "");
            return s.vec<Leaf>();
        }
    }
    
    std::vector<Leaf>        all_leafs(Sorted sorted)
    {
        return sorted ? all_leafs_sorted() : all_leafs_unsorted();
    }
    
    size_t              all_leafs_count()
    {
        static thread_local CacheQuery s("SELECT COUNT(1) FROM " TBL_LEAFS "");
        return s.size();
    }
    

    Atom                atom(Leaf l)
    {
        static thread_local CacheQuery s("SELECT atom FROM " TBL_LEAFS " WHERE id=? LIMIT 1");
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
        
        static thread_local CacheQuery i("INSERT INTO " TBL_LEAFS " (id,k) VALUES (?,?)");
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
    

    bool                exists(Leaf l)
    {
        return exists_leaf(l.id);
    }
    

    bool                exists_leaf(uint64_t i)
    {
        static thread_local CacheQuery s("SELECT 1 FROM " TBL_LEAFS " WHERE id=? LIMIT 1");
        return s.present(i);
    }

    
    Image               icon(Leaf l)
    {
        static thread_local CacheQuery    s("SELECT icon FROM " TBL_LEAFS " WHERE id=? LIMIT 1");
        return s.as<Image>(l.id);
    }
    

    Leaf::Info          info(Leaf l, bool autoKey)
    {
        Leaf::Info    ret;
        static thread_local CacheQuery s("SELECT k, title, atom, icon, abbr, brief FROM " TBL_LEAFS " WHERE id=?");
        auto s_af = s.af();
        s.bind(1, l.id);
        if(s.step() == SQResult::Row){
            ret.doc     = Document(l.id);
            ret.key     = s.v_string(1);
            ret.title   = s.v_string(2);
            if(autoKey && ret.title.empty())
                ret.title   = ret.key;
            ret.atom    = { s.v_uint64(3) };
            ret.icon    = { s.v_uint64(4) };
            ret.abbr    = s.v_string(5);
            ret.brief   = s.v_string(6);
        }
        return ret;
    }

    //bool                    is_leaf(Atom);
    
    
    std::string             key(Leaf l)
    {
        static thread_local CacheQuery s("SELECT k FROM " TBL_LEAFS " WHERE id=?");
        return s.str(l.id);
    }

    std::string             label(Leaf l)
    {
        static thread_local CacheQuery    s("SELECT ifnull(title,k) FROM " TBL_LEAFS " WHERE id=?");
        return s.str(l.id);
    }
    
    
    Leaf                leaf(uint64_t i)
    {
        return exists_leaf(i) ? Leaf{i} : Leaf();
    }

    Leaf                leaf(std::string_view  k)
    {
        static thread_local CacheQuery s("SELECT id FROM " TBL_LEAFS " WHERE k=?");
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
        static thread_local CacheQuery    s("SELECT id FROM " TBL_LEAFS " WHERE title like ? LIMIT 1");
        return s.as<Leaf>(k);
    }
    
    Leaf::SharedFile         leaf_doc(Fragment f, cdb_options_t opts)
    {
        if(!f)
            return Leaf::SharedFile();

        std::filesystem::path       fp  = path(f);

        const RootDir* rt  = root_dir(f);
        if(!rt)
            yWarning() << "No root_dir for: " << fp;

        Id::Lock  lk;
        if(!(opts & DONT_LOCK)){
            lk  = Id(f).lock(false);
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
        if(td->load(std::move(ch), fp) != std::error_code()){
            yError() << "Unable to read " << fp;
            return Leaf::SharedFile();
        }
        td -> set_file(fp);
        for(auto& ctx : td -> context)
            ctx.root_dir    = rt;
        return td;
    }

    
    Leaf::SharedData         merged(Leaf l, cdb_options_t opts)
    {
        if(!l)
            return Leaf::SharedData();
        
        Id::Lock   lk;
        if(!(opts & DONT_LOCK)){
            lk   = Id(l).lock(false);
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
    
    std::string             name(Leaf l)
    {
        return title(l);
    }

    NKI                 nki(Leaf l, bool autoKey)
    {
        static thread_local CacheQuery    s("SELECT title,icon,k FROM " TBL_LEAFS " WHERE id=?");
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

    Leaf::SharedFile         read(Leaf l, const RootDir* rt, cdb_options_t opts)
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

    std::vector<LeafFragDoc>   reads(Leaf l, class RootDir*rt, cdb_options_t opts)
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
        static thread_local CacheQuery s("SELECT 1 FROM " TBL_LEAF_TAG " WHERE leaf=? AND tag=? LIMIT 1");
        return s.present(l.id, t.id);
    }
    
    std::vector<Tag>            tags(Leaf l)
    {
        static thread_local CacheQuery s("SELECT tag FROM " TBL_LEAF_TAG " WHERE leaf=?");
        return s.vec<Tag>(l.id);
    }
    
    std::set<Tag>               tags_set(Leaf l)
    {
        static thread_local CacheQuery s("SELECT tag FROM " TBL_LEAF_TAG " WHERE leaf=?");
        return s.set<Tag>(l.id);
    }

    std::string             title(Leaf l)
    {
        static thread_local CacheQuery s("SELECT title FROM " TBL_LEAFS " WHERE id=?");
        return s.str(l.id);
    }



    Leaf::SharedFile         write(Leaf l, const RootDir* rt, cdb_options_t opts)
    {
        if(!l)
            return Leaf::SharedFile();
        Document    d   = document(l);
        if(!d){
            yWarning() << "write(Leaf '" << key(l) << "'): Has no document!";
            return Leaf::SharedFile();
        }
        if(rt && !rt->is_writable(DataRole::DB)){
            yWarning() << "write(Leaf '" << key(l) << "'): RootDir " << rt->key << " cannot be written to!";
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
