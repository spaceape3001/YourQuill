////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DrafterCDB.hpp"
#include <0/basic/ByteArray.hpp>
#include <0/io/FileUtils.hpp>
#include <0/basic/TextUtils.hpp>
#include <mithril/bit/NKI.hpp>
#include <mithril/document/DocumentCDB.hpp>
#include <mithril/folder/FolderCDB.hpp>
#include <mithril/fragment/FragmentCDB.hpp>
#include <mithril/root/RootDir.hpp>
#include <mithril/image/ImageCDB.hpp>
#include <mithril/drafter/DrafterFile.hpp>
#include <mithril/drafter/DrafterInfo.hpp>
#include <mithril/wksp/CacheQuery.hpp>

namespace yq::mithril::cdb {
    namespace {
        inline std::string drafter_filename(std::string_view k)
        {
            return make_filename(k, Drafter::EXTENSION);
        }
    }
    
    DrafterVector        all_drafters(Sorted sorted)
    {
        static thread_local CacheQuery qs("SELECT id FROM " TBL_DRAFTERS " ORDER BY k");
        static thread_local CacheQuery qu("SELECT id FROM " TBL_DRAFTERS "");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Drafter>();
    }
    
    size_t              all_drafters_count()
    {
        static thread_local CacheQuery s("SELECT COUNT(1) FROM " TBL_DRAFTERS "");
        return s.size();
    }
    
    
    Drafter                db_drafter(Document doc, bool*wasCreated)
    {
        if(wasCreated)
            *wasCreated = false;
            
        if(!doc)
            return Drafter();

        if(exists_drafter(doc.id))
            return Drafter{ doc.id };
        
        std::string     tk  = key(doc);
        std::string     k(strip_extension(tk));
        if(k.empty()){
            yError() << "Cannot create to blank key!";
            return Drafter();
        }
        
        static thread_local CacheQuery i("INSERT INTO " TBL_DRAFTERS " (id,k) VALUES (?,?)");
        auto i_af   = i.af();
        i.bind(1,doc.id);
        i.bind(2, k);
        if(i.exec()){
            if(wasCreated)
                *wasCreated = true;
            return Drafter(doc.id);
        } else if(exists_drafter(doc.id)){
            return Drafter(doc.id);
        } else {
            yError() << "Unable to create or find the drafter: " << k;
            return Drafter();
        }
    }

#if 0
    Folder              detail_folder(Drafter l)
    {
        Folder  f   = folder(Document{l.id});
        std::string sk  = skeyb(Document{l.id});
        return db_folder(f, sk+".d");
    }
#endif
    
    Document            document(Drafter l)
    {
        return exists(l) ?  Document{l.id} : Document{};
    }

    
    Drafter                drafter(uint64_t i)
    {
        return exists_drafter(i) ? Drafter{i} : Drafter();
    }

    Drafter                drafter(std::string_view  k)
    {
        static thread_local CacheQuery s("SELECT id FROM " TBL_DRAFTERS " WHERE k=?");
        return s.as<Drafter>(k);
    }

    Drafter                drafter(Document d, bool calc)
    {
        if(!d)
            return Drafter();
        if(exists_drafter(d.id))
            return Drafter(d.id);
        if(calc){
            std::string k   = key(folder(d)) + "/" + skeyb(d);
            return drafter(k);
        }
        return Drafter();
    }
    
    bool                exists(Drafter l)
    {
        return exists_drafter(l.id);
    }
    

    bool                exists_drafter(uint64_t i)
    {
        static thread_local CacheQuery s("SELECT 1 FROM " TBL_DRAFTERS " WHERE id=? LIMIT 1");
        return s.present(i);
    }

    
    Image               icon(Drafter l)
    {
        static thread_local CacheQuery    s("SELECT icon FROM " TBL_DRAFTERS " WHERE id=? LIMIT 1");
        return s.as<Image>(l.id);
    }
    

    Drafter::Info          info(Drafter l, bool autoKey)
    {
        Drafter::Info    ret;
        static thread_local CacheQuery s("SELECT k, name, icon, brief FROM " TBL_DRAFTERS " WHERE id=?");
        auto s_af = s.af();
        s.bind(1, l.id);
        if(s.step() == SQResult::Row){
            ret.doc     = Document(l.id);
            ret.key     = s.v_string(1);
            ret.name    = s.v_string(2);
            if(autoKey && ret.name.empty())
                ret.name   = ret.key;
            ret.icon    = { s.v_uint64(3) };
            ret.brief   = s.v_string(4);
        }
        return ret;
    }

    //bool                    is_drafter(Atom);
    
    
    std::string             key(Drafter l)
    {
        static thread_local CacheQuery s("SELECT k FROM " TBL_DRAFTERS " WHERE id=?");
        return s.str(l.id);
    }

    std::string             label(Drafter l)
    {
        static thread_local CacheQuery    s("SELECT ifnull(name,k) FROM " TBL_DRAFTERS " WHERE id=?");
        return s.str(l.id);
    }
    

    //Drafter                     make_drafter(std::string_view k, const RootDir* rt, cdb_options_t opts, bool* wasCreated)
    //{
        //if(wasCreated)
            //*wasCreated = false;
        //if(k.empty()){
            //yError() << "Cannot create a BLANK drafter.";
            //return Drafter();
        //}
    
        //if(!rt)
            //rt  = wksp::root_first(DataRole::Config);
        //if(!rt){
            //yError() << "No root_dir specified to create the drafter in!";
            //return Drafter{};
        //}
        
        //std::string     tfn = drafter_filename(k);
        //Document    doc = db_document(drafters_folder(), tfn);
        //bool            was = false;
        //Drafter         t   = db_drafter(doc, &was);
        //if(!t){
            //yWarning() << "Unable to create/find drafter: " << k;
            //return t;
        //}
        //if(wasCreated)
            //*wasCreated = was;
        //if(!was)
            //return t;
        //if(fragments_count(doc))
            //return t;
            
        //Id::Lock   lk;
        //if(!(opts & DONT_LOCK))
            //lk  = Id(t).lock(true);
        
            //// prelude, we're first....
        //Drafter::SharedFile td  = write(t, rt, opts);
        //td -> name  = k;
        //td -> save();
        //return t;
    //}

    
    Drafter::SharedData         merged(Drafter l, cdb_options_t opts)
    {
        if(!l)
            return Drafter::SharedData();
        
        Id::Lock   lk;
        if(!(opts & DONT_LOCK)){
            lk   = Id(l).lock(false);
            if(!lk){
                yWarning() << "Unable to acquire read lock on drafter: " << key(l);
                return Drafter::SharedData();
            }
        }

        Drafter::SharedData     ret = std::make_shared<Drafter::Data>();
        for(auto& i : reads(l, opts)){
            if(opts & IS_UPDATE)
                cdb::update(i.first);
            ret -> merge(*(i.second), static_cast<bool>(opts&OVERRIDE));
        }
        return ret;
    }
    
    std::string             name(Drafter l)
    {
        static thread_local CacheQuery    s("SELECT name FROM " TBL_DRAFTERS " WHERE id=?");
        return s.str(l);
    }

    NKI                 nki(Drafter l, bool autoKey)
    {
        static thread_local CacheQuery    s("SELECT name,icon,k FROM " TBL_DRAFTERS " WHERE id=?");
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

    Drafter::SharedFile         read(Drafter l, const RootDir* rt, cdb_options_t opts)
    {
        return drafter_doc(fragment(document(l), rt), opts);
    }

    std::vector<DrafterFragDoc>     reads(Drafter l, cdb_options_t opts)
    {
        std::vector<DrafterFragDoc>  ret;
        for(Fragment f : fragments(document(l), DataRole::DB)){
            Drafter::SharedFile p   = drafter_doc(f, opts);
            if(p)
                ret.push_back(DrafterFragDoc(f,p));
        }
        return ret;
    }

    std::vector<DrafterFragDoc>   reads(Drafter l, class RootDir*rt, cdb_options_t opts)
    {
        std::vector<DrafterFragDoc>  ret;
        for(Fragment f : fragments(document(l), rt)){
            Drafter::SharedFile p   = drafter_doc(f, opts);
            if(p)
                ret.push_back(DrafterFragDoc(f,p));
        }
        return ret;
    }



    
    #if 0
    Drafter                drafter(Folder f)
    {
        return drafter(key(f));
    }
    #endif
    
    Drafter::SharedFile         drafter_doc(Fragment f, cdb_options_t opts)
    {
        if(!f)
            return Drafter::SharedFile();

        std::filesystem::path       fp  = path(f);

        const RootDir* rt  = root_dir(f);
        if(!rt)
            yWarning() << "No root_dir for: " << fp;

        Id::Lock  lk;
        if(!(opts & DONT_LOCK)){
            lk  = Id(f).lock(false);
            if(!lk){
                yWarning() << "Unable to get read lock on fragment: " << fp;
                return Drafter::SharedFile();
            }
        }

        auto    ch   = file_bytes(fp);
        lk.free();
        if(ch.empty()){
            if(opts & ALLOW_EMPTY)
                return std::make_shared<Drafter::File>() ;
            return Drafter::SharedFile();
        }
        
        Drafter::SharedFile     td  = std::make_shared<Drafter::File>();
        if(td->load(std::move(ch), fp) != std::error_code()){
            yError() << "Unable to read " << fp;
            return Drafter::SharedFile();
        }
        td -> set_file(fp);
        //for(auto& ctx : td -> context)
            //ctx.root_dir    = rt;
        return td;
    }


    Drafter::SharedFile         write(Drafter l, const RootDir* rt, cdb_options_t opts)
    {
        if(!l)
            return Drafter::SharedFile();
        Document    d   = document(l);
        if(!d){
            yWarning() << "write(Drafter '" << key(l) << "'): Has no document!";
            return Drafter::SharedFile();
        }
        if(rt && !rt->is_writable(DataRole::DB)){
            yWarning() << "write(Drafter '" << key(l) << "'): RootDir " << rt->key << " cannot be written to!";
            return Drafter::SharedFile();
        }

        Fragment    f   = rt ? fragment(d, rt) : writable(d, DataRole::DB);
        if(f)
            return drafter_doc(f, opts | ALLOW_EMPTY);
            
        Folder      fo  = folder(d);
        if((fo != cdb::top_folder()) && !exists(fo, rt))
            make_directory(fo, rt);
        Drafter::SharedFile ptr  = std::make_shared<Drafter::File>();
        ptr -> set_file( rt -> resolve(key(d)));
        ptr -> reload();
        return ptr;
    }

}
