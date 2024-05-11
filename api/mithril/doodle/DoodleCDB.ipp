////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DoodleCDB.hpp"
#include <0/basic/ByteArray.hpp>
#include <0/io/FileUtils.hpp>
#include <0/basic/TextUtils.hpp>
#include <mithril/bit/NKI.hpp>
#include <mithril/document/DocumentCDB.hpp>
#include <mithril/folder/FolderCDB.hpp>
#include <mithril/fragment/FragmentCDB.hpp>
#include <mithril/root/RootDir.hpp>
#include <mithril/image/ImageCDB.hpp>
//#include <mithril/doodle/DoodleFile.hpp>
#include <mithril/doodle/DoodleInfo.hpp>
#include <mithril/wksp/CacheQuery.hpp>

namespace yq::mithril::cdb {
    namespace {
        inline std::string doodle_filename(std::string_view k)
        {
            return make_filename(k, Doodle::EXTENSION);
        }
    }
    
    DoodleVector        all_doodles(Sorted sorted)
    {
        static thread_local CacheQuery qs("SELECT id FROM " TBL_DOODLES " ORDER BY k");
        static thread_local CacheQuery qu("SELECT id FROM " TBL_DOODLES "");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Doodle>();
    }
    
    size_t              all_doodles_count()
    {
        static thread_local CacheQuery s("SELECT COUNT(1) FROM " TBL_DOODLES "");
        return s.size();
    }
    
    
    Doodle                db_doodle(Document doc, bool*wasCreated)
    {
        if(wasCreated)
            *wasCreated = false;
            
        if(!doc)
            return Doodle();

        if(exists_doodle(doc.id))
            return Doodle{ doc.id };
        
        std::string     tk  = key(doc);
        std::string     k(strip_extension(tk));
        if(k.empty()){
            yError() << "Cannot create to blank key!";
            return Doodle();
        }
        
        static thread_local CacheQuery i("INSERT INTO " TBL_DOODLES " (id,k) VALUES (?,?)");
        auto i_af   = i.af();
        i.bind(1,doc.id);
        i.bind(2, k);
        if(i.exec()){
            if(wasCreated)
                *wasCreated = true;
            return Doodle(doc.id);
        } else if(exists_doodle(doc.id)){
            return Doodle(doc.id);
        } else {
            yError() << "Unable to create or find the doodle: " << k;
            return Doodle();
        }
    }

#if 0
    Folder              detail_folder(Doodle l)
    {
        Folder  f   = folder(Document{l.id});
        std::string sk  = skeyb(Document{l.id});
        return db_folder(f, sk+".d");
    }
#endif
    
    Document            document(Doodle l)
    {
        return exists(l) ?  Document{l.id} : Document{};
    }

    
    Doodle                doodle(uint64_t i)
    {
        return exists_doodle(i) ? Doodle{i} : Doodle();
    }

    Doodle                doodle(std::string_view  k)
    {
        static thread_local CacheQuery s("SELECT id FROM " TBL_DOODLES " WHERE k=?");
        return s.as<Doodle>(k);
    }

    Doodle                doodle(Document d, bool calc)
    {
        if(!d)
            return Doodle();
        if(exists_doodle(d.id))
            return Doodle(d.id);
        if(calc){
            std::string k   = key(folder(d)) + "/" + skeyb(d);
            return doodle(k);
        }
        return Doodle();
    }
    
    bool                exists(Doodle l)
    {
        return exists_doodle(l.id);
    }
    

    bool                exists_doodle(uint64_t i)
    {
        static thread_local CacheQuery s("SELECT 1 FROM " TBL_DOODLES " WHERE id=? LIMIT 1");
        return s.present(i);
    }

    
    Image               icon(Doodle l)
    {
        static thread_local CacheQuery    s("SELECT icon FROM " TBL_DOODLES " WHERE id=? LIMIT 1");
        return s.as<Image>(l.id);
    }
    

    Doodle::Info          info(Doodle l, bool autoKey)
    {
        Doodle::Info    ret;
        static thread_local CacheQuery s("SELECT k, name, icon FROM " TBL_DOODLES " WHERE id=?");
        auto s_af = s.af();
        s.bind(1, l.id);
        if(s.step() == SQResult::Row){
            ret.doc     = Document(l.id);
            ret.key     = s.v_string(1);
            ret.name    = s.v_string(2);
            if(autoKey && ret.name.empty())
                ret.name   = ret.key;
            ret.icon    = { s.v_uint64(3) };
            //ret.brief   = s.v_string(4);
        }
        return ret;
    }

    //bool                    is_doodle(Atom);
    
    
    std::string             key(Doodle l)
    {
        static thread_local CacheQuery s("SELECT k FROM " TBL_DOODLES " WHERE id=?");
        return s.str(l.id);
    }

    std::string             label(Doodle l)
    {
        static thread_local CacheQuery    s("SELECT ifnull(name,k) FROM " TBL_DOODLES " WHERE id=?");
        return s.str(l.id);
    }
    

    //Doodle                     make_doodle(std::string_view k, const RootDir* rt, cdb_options_t opts, bool* wasCreated)
    //{
        //if(wasCreated)
            //*wasCreated = false;
        //if(k.empty()){
            //yError() << "Cannot create a BLANK doodle.";
            //return Doodle();
        //}
    
        //if(!rt)
            //rt  = wksp::root_first(DataRole::Config);
        //if(!rt){
            //yError() << "No root_dir specified to create the doodle in!";
            //return Doodle{};
        //}
        
        //std::string     tfn = doodle_filename(k);
        //Document    doc = db_document(doodles_folder(), tfn);
        //bool            was = false;
        //Doodle         t   = db_doodle(doc, &was);
        //if(!t){
            //yWarning() << "Unable to create/find doodle: " << k;
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
        //Doodle::SharedFile td  = write(t, rt, opts);
        //td -> name  = k;
        //td -> save();
        //return t;
    //}

#if 0
    
    Doodle::SharedData         merged(Doodle l, cdb_options_t opts)
    {
        if(!l)
            return Doodle::SharedData();
        
        Id::Lock   lk;
        if(!(opts & DONT_LOCK)){
            lk   = Id(l).lock(false);
            if(!lk){
                yWarning() << "Unable to acquire read lock on doodle: " << key(l);
                return Doodle::SharedData();
            }
        }

        Doodle::SharedData     ret = std::make_shared<Doodle::Data>();
        for(auto& i : reads(l, opts)){
            if(opts & IS_UPDATE)
                cdb::update(i.first);
            ret -> merge(*(i.second), static_cast<bool>(opts&OVERRIDE));
        }
        return ret;
    }
#endif
    
    std::string             name(Doodle l)
    {
        static thread_local CacheQuery    s("SELECT name FROM " TBL_DOODLES " WHERE id=?");
        return s.str(l);
    }

    NKI                 nki(Doodle l, bool autoKey)
    {
        static thread_local CacheQuery    s("SELECT name,icon,k FROM " TBL_DOODLES " WHERE id=?");
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
    Doodle::SharedFile         read(Doodle l, const RootDir* rt, cdb_options_t opts)
    {
        return doodle_doc(fragment(document(l), rt), opts);
    }

    std::vector<DoodleFragDoc>     reads(Doodle l, cdb_options_t opts)
    {
        std::vector<DoodleFragDoc>  ret;
        for(Fragment f : fragments(document(l), DataRole::DB)){
            Doodle::SharedFile p   = doodle_doc(f, opts);
            if(p)
                ret.push_back(DoodleFragDoc(f,p));
        }
        return ret;
    }

    std::vector<DoodleFragDoc>   reads(Doodle l, class RootDir*rt, cdb_options_t opts)
    {
        std::vector<DoodleFragDoc>  ret;
        for(Fragment f : fragments(document(l), rt)){
            Doodle::SharedFile p   = doodle_doc(f, opts);
            if(p)
                ret.push_back(DoodleFragDoc(f,p));
        }
        return ret;
    }
#endif


    
    #if 0
    Doodle                doodle(Folder f)
    {
        return doodle(key(f));
    }
    #endif
    
    #if 0
    Doodle::SharedFile         doodle_doc(Fragment f, cdb_options_t opts)
    {
        if(!f)
            return Doodle::SharedFile();

        std::filesystem::path       fp  = path(f);

        const RootDir* rt  = root_dir(f);
        if(!rt)
            yWarning() << "No root_dir for: " << fp;

        Id::Lock  lk;
        if(!(opts & DONT_LOCK)){
            lk  = Id(f).lock(false);
            if(!lk){
                yWarning() << "Unable to get read lock on fragment: " << fp;
                return Doodle::SharedFile();
            }
        }

        auto    ch   = file_bytes(fp);
        lk.free();
        if(ch.empty()){
            if(opts & ALLOW_EMPTY)
                return std::make_shared<Doodle::File>() ;
            return Doodle::SharedFile();
        }
        
        Doodle::SharedFile     td  = std::make_shared<Doodle::File>();
        if(td->load(std::move(ch), fp) != std::error_code()){
            yError() << "Unable to read " << fp;
            return Doodle::SharedFile();
        }
        td -> set_file(fp);
        //for(auto& ctx : td -> context)
            //ctx.root_dir    = rt;
        return td;
    }
    #endif

#if 0
    Doodle::SharedFile         write(Doodle l, const RootDir* rt, cdb_options_t opts)
    {
        if(!l)
            return Doodle::SharedFile();
        Document    d   = document(l);
        if(!d){
            yWarning() << "write(Doodle '" << key(l) << "'): Has no document!";
            return Doodle::SharedFile();
        }
        if(rt && !rt->is_writable(DataRole::DB)){
            yWarning() << "write(Doodle '" << key(l) << "'): RootDir " << rt->key << " cannot be written to!";
            return Doodle::SharedFile();
        }

        Fragment    f   = rt ? fragment(d, rt) : writable(d, DataRole::DB);
        if(f)
            return doodle_doc(f, opts | ALLOW_EMPTY);
            
        Folder      fo  = folder(d);
        if((fo != cdb::top_folder()) && !exists(fo, rt))
            make_directory(fo, rt);
        Doodle::SharedFile ptr  = std::make_shared<Doodle::File>();
        ptr -> set_file( rt -> resolve(key(d)));
        ptr -> reload();
        return ptr;
    }
#endif
}
