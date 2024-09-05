////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DoodleCDB.hpp"
#include <yq-toolbox/container/ByteArray.hpp>
#include <yq-toolbox/io/FileUtils.hpp>

#include <mithril/bit/NKI.hpp>
#include <mithril/document/DocumentCDB.hpp>
#include <mithril/doodle/DoodleInfo.hpp>
#include <mithril/folder/FolderCDB.hpp>
#include <mithril/fragment/FragmentCDB.hpp>
#include <mithril/root/RootDir.hpp>
#include <mithril/image/ImageCDB.hpp>
#include <mithril/tag/Tag.hpp>
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
    
    
    Doodle                db_doodle(Fragment frag, bool*wasCreated)
    {
        if(wasCreated)
            *wasCreated = false;
            
        if(!frag)
            return Doodle();

        if(exists_doodle(frag.id))
            return Doodle{ frag.id };
        
        static thread_local CacheQuery i("INSERT INTO " TBL_DOODLES " (id) VALUES (?)");
        auto i_af   = i.af();
        i.bind(1,frag.id);
        if(i.exec()){
            if(wasCreated)
                *wasCreated = true;
            return Doodle(frag.id);
        } else if(exists_doodle(frag.id)){
            return Doodle(frag.id);
        } else {
            yError() << "Unable to create or find the doodle: " << key(frag);
            return Doodle();
        }
    }
    
    Document            document(Doodle l)
    {
        return document(fragment(l));
    }

    
    Doodle                doodle(uint64_t i)
    {
        return exists_doodle(i) ? Doodle{i} : Doodle();
    }

    Doodle                doodle(std::string_view  k)
    {
        return doodle(document(k));
    }

    Doodle                doodle(Document d)
    {
        return doodle(first_fragment(d, DataRole::Doodles));
    }

    Doodle                doodle(Fragment frag, bool calc)
    {
        //   calc TBD
        return doodle(frag.id);
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

    Fragment            fragment(Doodle l)
    {
        return exists(l) ?  Fragment{l.id} : Fragment{};
    }

    
    Image               icon(Doodle l)
    {
        static thread_local CacheQuery    s("SELECT icon FROM " TBL_DOODLES " WHERE id=? LIMIT 1");
        return s.as<Image>(l.id);
    }
    

    Doodle::Info          info(Doodle l, bool autoKey)
    {
        Doodle::Info    ret;
        static thread_local CacheQuery s("SELECT title, icon FROM " TBL_DOODLES " WHERE id=?");
        auto s_af = s.af();
        s.bind(1, l.id);
        if(s.step() == SQResult::Row){
            ret.name    = s.v_string(1);
            if(autoKey && ret.name.empty())
                ret.name   = key(l);
            ret.icon    = { s.v_uint64(2) };
        }
        return ret;
    }

    
    std::string             key(Doodle l)
    {
        return key(document(l));
    }

    std::string             label(Doodle l)
    {
        std::string     s = name(l);
        if(s.empty())
            s = key(l);
        return s;
    }
    
    std::string             name(Doodle l)
    {
        return title(l);
    }
    
    NKI                 nki(Doodle l, bool autoKey)
    {
        static thread_local CacheQuery    s("SELECT title, icon FROM " TBL_DOODLES " WHERE id=?");
        auto s_af = s.af();
        s.bind(1, l.id);
        if(s.step() == SQResult::Row){
            NKI  ret;
            ret.name    = s.v_string(1);
            ret.icon    = Image(s.v_uint64(2)) ;
            ret.key     = key(l);
            if(autoKey && ret.name.empty())
                ret.name    = ret.key;
            return ret;
        }
        return NKI{};
    }

    bool                        tagged(Doodle l, Tag t)
    {
        static thread_local CacheQuery s("SELECT 1 FROM " TBL_DOODLE_TAG " WHERE doodle=? AND tag=? LIMIT 1");
        return s.present(l.id, t.id);
    }
    
    TagVector            tags(Doodle l)
    {
        static thread_local CacheQuery s("SELECT tag FROM " TBL_DOODLE_TAG " WHERE doodle=?");
        return s.vec<Tag>(l.id);
    }
    
    TagSet               tags_set(Doodle l)
    {
        static thread_local CacheQuery s("SELECT tag FROM " TBL_DOODLE_TAG " WHERE doodle=?");
        return s.set<Tag>(l.id);
    }

    std::string             title(Doodle l)
    {
        static thread_local CacheQuery    s("SELECT title FROM " TBL_DOODLES " WHERE id=?");
        return s.str(l);
    }

}
