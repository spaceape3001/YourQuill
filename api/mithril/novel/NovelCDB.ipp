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
    
    Novel                db_novel(Fragment frag, bool*wasCreated)
    {
        if(wasCreated)
            *wasCreated = false;
            
        if(!frag)
            return Novel();

        if(exists_novel(frag.id))
            return Novel{frag.id };
        
        static thread_local CacheQuery i("INSERT INTO " TBL_NOVELS " (id) VALUES (?)");
        auto i_af   = i.af();
        i.bind(1,frag.id);
        if(i.exec()){
            if(wasCreated)
                *wasCreated = true;
            return Novel(frag.id);
        } else if(exists_novel(frag.id)){
            return Novel(frag.id);
        } else {
            yError() << "Unable to create or find the novel: " << key(frag);
            return Novel();
        }
    }

    Document            document(Novel l)
    {
        return document(fragment(l));
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
    
    Fragment            fragment(Novel l)
    {
        return exists(l) ?  Fragment{l.id} : Fragment{};
    }

    
    Image               icon(Novel l)
    {
        static thread_local CacheQuery    s("SELECT icon FROM " TBL_NOVELS " WHERE id=? LIMIT 1");
        return s.as<Image>(l.id);
    }
    

    Novel::Info          info(Novel l, bool autoKey)
    {
        Novel::Info    ret;
        static thread_local CacheQuery s("SELECT title, icon  FROM " TBL_NOVELS " WHERE id=?");
        auto s_af = s.af();
        s.bind(1, l.id);
        if(s.step() == SQResult::Row){
            ret.title   = s.v_string(1);
            if(autoKey && ret.title.empty())
                ret.title   = key(l);
            ret.icon    = { s.v_uint64(2) };
        }
        return ret;
    }

    std::string             key(Novel l)
    {
        return key(document(l));
    }

    std::string             label(Novel l)
    {
        std::string     s = title(l);
        if(s.empty())
            s = key(l);
        return s;
    }
    
    
    Novel                novel(uint64_t i)
    {
        return exists_novel(i) ? Novel{i} : Novel();
    }

    Novel                novel(std::string_view  k)
    {
        return novel(document(k));
    }

    Novel                novel(Document d)
    {
        return novel(first_fragment(d, DataRole::Novels));
    }
    
    Novel               novel(Fragment frag, bool calc)
    {
        // TODO -- the calc bit
        return novel(frag.id);
    }
    
    Novel                    novel_by_title(std::string_view k)
    {
        static thread_local CacheQuery    s("SELECT id FROM " TBL_NOVELS " WHERE title like ? LIMIT 1");
        return s.as<Novel>(k);
    }
    
    std::string             name(Novel l)
    {
        return title(l);
    }

    NKI                 nki(Novel l, bool autoKey)
    {
        static thread_local CacheQuery    s("SELECT title,iconFROM " TBL_NOVELS " WHERE id=?");
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
}
