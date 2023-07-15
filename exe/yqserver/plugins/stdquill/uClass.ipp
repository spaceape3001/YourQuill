////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/CollectionUtils.hpp>

#include <mithril/category/CategoryCDB.hpp>
#include <mithril/class/ClassCDB.hpp>
#include <mithril/class/ClassData.hpp>
#include <mithril/document/DocumentCDB.hpp>
#include <mithril/enum/Change.hpp>
#include <mithril/fragment/FragmentCDB.hpp>
#include <mithril/image/ImageCDB.hpp>
#include <mithril/tag/TagCDB.hpp>
#include <mithril/wksp/CacheQuery.hpp>

using namespace yq;
using namespace yq::mithril;

namespace {


    void                u_class_erase(Class x)
    {
        static thread_local CacheQuery stmts[] = {
            CacheQuery("DELETE FROM CAlias WHERE class=?"),
            CacheQuery("DELETE FROM CPrefix WHERE class=?"),
            CacheQuery("DELETE FROM CSuffix WHERE class=?"),
            CacheQuery("DELETE FROM CTags WHERE class=?"),
            CacheQuery("DELETE FROM CLookup WHERE class=? AND priority=1"),
            CacheQuery("DELETE FROM Classes WHERE id=?")
        };
        
        if(!x)
            return ;
            
        for(auto& sq : stmts)
            sq.exec(x.id);
    }

    void                u_class_erase(Document doc)
    {
        u_class_erase(cdb::class_(doc));
    }

    void                u_class_icon(Class x)
    {
        if(!x)
            return ;
            
        static thread_local CacheQuery u1("UPDATE Classes SET icon=? WHERE id=?");
        static thread_local CacheQuery u2("UPDATE Documents SET icon=? WHERE id=?");

        Document    doc     = cdb::document(x);
        Image       img     = cdb::best_image(doc);
        if(img != cdb::icon(x)){
                // we'll eventually do inheritance....
            u1.exec(img.id, x.id);
            u2.exec(doc.id, x.id);
        }
    }

    void    u_class_notify(Fragment frag, Change chg)
    {
        Document    doc = cdb::document(frag);
        if(chg == Change::Removed){
            if(cdb::fragments_count(doc) <= 1){
                u_class_erase(doc);
                return ;
            }
        }

        bool        created = false;
        Class       x   = cdb::db_class(doc, &created);
        if(!x)
            return ;

        if(!x)
            return;
        
        Class::SharedData data = cdb::merged(x, cdb::DONT_LOCK|cdb::IS_UPDATE);
        if(!data)
            return;
        
        Category    cat;
        if(!data->category.empty())
            cat = cdb::db_category(data->category);

        static thread_local CacheQuery uInfo("UPDATE Classes SET name=?, plural=?, brief=?, category=?, binding=? WHERE id=?");

        uInfo.bind(1, data->name);
        uInfo.bind(2, data->plural);
        uInfo.bind(3, data->brief);
        uInfo.bind(4, cat.id);
        uInfo.bind(5, data->binding);
        uInfo.bind(6, x.id);
        uInfo.exec();

        
        static thread_local CacheQuery iTag("INSERT INTO CTags (class, tag) VALUES (?,?)");
        static thread_local CacheQuery dTag("DELETE FROM CTags WHERE class=? AND tag=?");

        std::set<Tag>   new_tags = cdb::find_tags_set(data->tags, true);
        
        if(created){
            u_class_icon(x);
            for(Tag t : new_tags)
                iTag.exec(x.id, t.id);

            //  TODO inheritance....
            
        } else {
            std::set<Tag>   old_tags = make_set(cdb::tags(x));
            auto ch_tag = add_remove(old_tags, new_tags);
            for(Tag t : ch_tag.added)
                iTag.exec(x.id, t.id);
            for(Tag t : ch_tag.removed)
                dTag.exec(x.id, t.id);
                
            // Do class analysis here

            //  TODO inheritance....
        }
    }

    void    u_class_notify_icons(Fragment frag, Change)
    {
        u_class_icon(cdb::class_(cdb::document(frag), true));
    }

    void    u_class_stage3_pass1_create(Document doc)
    {
        std::string k   = cdb::base_key(doc);
        if(k.empty())   //  skipping, no key
            return ;

        //  First, do a manual merge
        auto dp     = std::make_shared<Class::Data>();
        for(Fragment f : cdb::fragments(doc, DataRole::Config)){
            Class::SharedFile    p   = class_doc(f, cdb::DONT_LOCK);
            if(!p)
                continue;
            cdb::update(f);
            dp->merge(*p, false);
        }
        
        Category    cat;
        if(!dp->category.empty())
            cat = cdb::db_category(dp->category);
        
        std::string name    = dp->name.empty() ? k : dp->name;
        std::string plural  = dp->plural.empty() ? ( name + 's' ) : name;
        
        Image       img     = cdb::best_image(doc);

        static thread_local CacheQuery iInfo("INSERT OR REPLACE INTO Classes (id,k,name,plural,brief,category,binding,icon,url,devurl) VALUES (?,?,?,?,?,?,?,?,?,?)");
        iInfo.bind(1, doc.id);
        iInfo.bind(2, k);
        iInfo.bind(3, name);
        iInfo.bind(4, plural);
        iInfo.bind(5, dp->brief);
        iInfo.bind(6, cat.id);
        iInfo.bind(7, dp->binding); // Maybe....?
        iInfo.bind(8, img.id);
        iInfo.bind(9, dp->url);
        iInfo.bind(10, dp->dev_url);
        iInfo.exec();
        
        Class   x{doc.id};
        
        //[[maybe_unused]] UClass& u   = UClass::get(x);
        
        static thread_local CacheQuery iTag("INSERT INTO CTags (class, tag) VALUES (?, ?)");
        for(std::string_view tk : dp->tags){
            Tag     t   = cdb::db_tag(tk);
            if(!t){
                yWarning() << "Uncreatable tag " << tk << " referenced in class " << k;
                continue;
            }
            iTag.bind(1, doc.id);
            iTag.bind(2, t.id);
            iTag.exec();
        }
        
        static thread_local CacheQuery iAlias("INSERT INTO CAlias (class, alias) VALUES (?,?)");
        
        for(auto& p : dp->prefixes){
            std::string a   = p+k;
            iAlias.exec(x.id, a);
         
            for(auto& s : dp->suffixes){
                a   = p+k+s;
                iAlias.exec(x.id, a);
            }
        }
        
        for(auto& s : dp->suffixes){
            std::string a   = k+s;
            iAlias.exec(x.id, a);
        }
        
        for(auto& aa : dp->aliases){
            iAlias.exec(x.id, aa);

            for(auto& p : dp->prefixes){
                std::string a   = p+aa;
                iAlias.exec(x.id, a);
             
                for(auto& s : dp->suffixes){
                    a   = p+aa+s;
                    iAlias.exec(x.id, a);
                }
            }
            
            for(auto& s : dp->suffixes){
                std::string a   = aa+s;
                iAlias.exec(x.id, a);
            }
        }
        
        static thread_local CacheQuery iPrefix("INSERT INTO CPrefix (class, prefix) VALUES (?,?)");
        for(auto& p : dp->prefixes)
            iPrefix.exec(x.id, p);
            
        static thread_local CacheQuery iSuffix("INSERT INTO CSuffix (class, suffix) VALUES (?,?)");
        for(auto& s : dp->suffixes)
            iSuffix.exec(x.id, s);
        
    }

    void    u_class_stage3_pass2_bind(Document doc)
    {
        Class   x       = cdb::db_class(doc);
        auto dp         = cdb::merged(x, cdb::DONT_LOCK);
        if(!dp)
            return ;
        
        static thread_local CacheQuery iUse("INSERT INTO CDepends (class, base) VALUES (?,?)");
        for(std::string_view ck : dp->use){
            Class   y   = cdb::db_class(ck);
            if(!y){
                yWarning() << "Uncreatable USE " << ck << " referenced in class " << cdb::key(x);
                continue;
            }
            
            iUse.bind(1, x.id);
            iUse.bind(2, y.id);
            iUse.exec();
        }
        
        static thread_local CacheQuery iTarget("INSERT INTO CTargets (class, target) VALUES (?,?)");
        for(std::string_view ck : dp->targets){
            Class   y = cdb::db_class(ck);
            if(!y){
                yWarning() << "Uncreatable TARGET " << ck << " referenced in class " << cdb::key(x);
                continue;
            }

            iTarget.bind(1, x.id);
            iTarget.bind(2, y.id);
            iTarget.exec();
        }
        
        static thread_local CacheQuery iSource("INSERT INTO CSources (class, source) VALUES (?,?)");
        for(std::string_view ck : dp->sources){
            Class   y = cdb::db_class(ck);
            if(!y){
                yWarning() << "Uncreatable SOURCE " << ck << " referenced in class " << cdb::key(x);
                continue;
            }

            iSource.bind(1, x.id);
            iSource.bind(2, y.id);
            iSource.exec();
        }

    #if 0
        static thread_local CacheQuery iReverse("INSERT INTO CReverses (class, reverse) VALUES (?,?)");
        for(std::string_view ck : dp->reverse){
            Class   y = cdb::db_class(ck);
            if(!y){
                yWarning() << "Uncreatable REVERSE " << ck << " referenced in class " << cdb::key(x);
                continue;
            }

            iReverse.bind(1, x.id);
            iReverse.bind(2, y.id);
            iReverse.exec();
        }
    #endif

    }

    void    u_class_stage3_pass3_extend(Document doc)
    {
        Class x = cdb::db_class(doc);
        if(!x)
            return;


        static thread_local CacheQuery iUse("INSERT INTO CDepends (class, base, hops) VALUES (?,?,?)");

        ClassCountMap   bases  = cdb::make_count_map(cdb::base_classes_ranked(x));
        ClassCountMap   copy    = bases;
        for(auto & itr : copy){
            for(auto& cr : cdb::base_classes_ranked(itr.first)){
                if(bases.contains(cr.cls))        // already in our base list
                    continue;
                uint64_t    cnt   = 1ULL + itr.second + cr.rank;
                bases[cr.cls]     = HCountU64{ cnt };  // insert so we don't override
            }
        }

        for(auto & itr : bases){
            if(!itr.second.cnt)
                continue;
                
            iUse.bind(1, x.id);
            iUse.bind(2, itr.first.id);
            iUse.bind(3, itr.second.cnt);
            iUse.exec();
            iUse.reset();
        }
    }

    void    u_class_stage3_pass4_deduce(Document doc)
    {
        Class x = cdb::db_class(doc);
        if(!x)
            return;
            
            
        /* This should be the finalize step... */
        
        ClassCountMap   bases  = cdb::make_count_map(cdb::base_classes_ranked(x));
        
    }

#if 0
    void    u_class_update(Class x)
    {
    }
#endif
}