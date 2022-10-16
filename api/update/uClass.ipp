////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uClass.hpp"
#include <basic/Logging.hpp>
#include <kernel/atom/ClassCDB.hpp>
#include <kernel/atom/ClassData.hpp>
#include <kernel/db/SQ.hpp>
#include <kernel/file/DocumentCDB.hpp>
#include <kernel/file/FragmentCDB.hpp>
#include <kernel/image/ImageCDB.hpp>
#include <kernel/org/CategoryCDB.hpp>
#include <kernel/org/TagCDB.hpp>

namespace yq {
    namespace update {
        void                class_erase(Class x)
        {
            using cdb::SQ;
            
            static thread_local cdb::SQ stmts[] = {
                SQ("DELETE FROM CAlias WHERE class=?"),
                SQ("DELETE FROM CPrefix WHERE class=?"),
                SQ("DELETE FROM CSuffix WHERE class=?"),
                SQ("DELETE FROM CTags WHERE class=?"),
                SQ("DELETE FROM CLookup WHERE class=? AND priority=1"),
                SQ("DELETE FROM Classes WHERE id=?")
            };
            
            if(!x)
                return ;
                
            for(auto& sq : stmts)
                sq.exec(x.id);
        }
        
        void                class_erase(Document doc)
        {
            class_erase(cdb::class_(doc));
        }

        void                class_icon(Class x)
        {
            if(!x)
                return ;
                
            static thread_local cdb::SQ u1("UPDATE Classes SET icon=? WHERE id=?");
            static thread_local cdb::SQ u2("UPDATE Documents SET icon=? WHERE id=?");

            Document    doc     = cdb::document(x);
            Image       img     = cdb::best_image(doc);
            if(img != cdb::icon(x)){
                    // we'll eventually do inheritance....
                u1.exec(img.id, x.id);
                u2.exec(doc.id, x.id);
            }
        }

        void    class_notify_icons(Fragment frag, Change)
        {
            class_icon(cdb::class_(cdb::document(frag), true));
        }

        void    class_stage3_pass1_create(Document doc)
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
            
            static thread_local cdb::SQ iInfo("INSERT OR REPLACE INTO Classes (id,k,name,plural,brief,category,binding) VALUES (?,?,?,?,?,?,?)");
            iInfo.bind(1, doc.id);
            iInfo.bind(2, k);
            iInfo.bind(3, name);
            iInfo.bind(4, plural);
            iInfo.bind(5, dp->brief);
            iInfo.bind(6, cat.id);
            iInfo.bind(7, dp->binding); // Maybe....?
            iInfo.exec();
            
            static thread_local cdb::SQ iTag("INSERT INTO CTags (class, tag) VALUES (?, ?)");
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
        }

        void    class_stage3_pass2_bind(Document doc)
        {
            Class   x       = cdb::db_class(doc);
            auto dp         = cdb::merged(x, cdb::DONT_LOCK);
            if(!dp)
                return ;
            
            static thread_local cdb::SQ iUse("INSERT INTO CDepends (class, base, hops) VALUES (?,?,0)");
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
            
            static thread_local cdb::SQ iTarget("INSERT INTO CTargets (class, target, hops) VALUES (?,?,0)");
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
            
            static thread_local cdb::SQ iSource("INSERT INTO CSources (class, source, hops) VALUES (?,?,0)");
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

            static thread_local cdb::SQ iReverse("INSERT INTO CReverses (class, reverse, hops) VALUES (?,?,0)");
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
        }
        
        void    class_stage3_pass3_extend(Document doc)
        {
            Class x = cdb::db_class(doc);
            if(!x)
                return;


            static thread_local cdb::SQ iUse("INSERT INTO CDepends (class, base, hops) VALUES (?,?,?)");

            ClassCountMap   bases  = cdb::make_count_map(cdb::uses_ranked(x));
            for(auto & itr : bases){
                for(auto& cr : cdb::uses_ranked(itr.first)){
                    if(bases.contains(cr.cls))        // already in our base list
                        continue;
                    uint64_t    cnt   = 1ULL + itr.second + cr.rank;
                    bases[cr.cls]     = HCountU64{ cnt };  // insert so we don't override
                    iUse.bind(1, x.id);
                    iUse.bind(2, cr.cls.id);
                    iUse.bind(3, cnt);
                    iUse.exec();
                    iUse.reset();
                }
            }
        }
        
    }
}

