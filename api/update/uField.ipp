////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uField.hpp"
#include <basic/Logging.hpp>
#include <basic/meta/TypeInfo.hpp>
#include <kernel/atom/FieldCDB.hpp>
#include <kernel/atom/FieldData.hpp>
#include <kernel/db/SQ.hpp>
#include <kernel/file/DocumentCDB.hpp>
#include <kernel/file/FragmentCDB.hpp>

namespace yq {
    namespace update {
        void    field_erase(Field x)
        {
            static thread_local cdb::SQ stmts[] = {
                cdb::SQ("DELETE FROM CFields WHERE field=?"),
                cdb::SQ("DELETE FROM FTags WHERE field=?"),
                cdb::SQ("DELETE FROM Fields WHERE id=?")
            };

            if(!x)
                return ;
            
            for(auto& sq : stmts)
                sq.exec(x.id);
        }

        void    field_erase(Document doc)
        {
            field_erase(cdb::field(doc));
        }
    
        void    field_icon(Field x)
        {
            if(!x)
                return ;
                
            Document    doc     = cdb::document(x);
            Image       img     = cdb::best_image(doc);
            
            static thread_local cdb::SQ u1("UPDATE Fields SET icon=? WHERE id=?");
            static thread_local cdb::SQ u2("UPDATE Documents SET icon=? WHERE id=?");

            if(cdb::icon(x) != img){
                u1.exec(img.id, x.id);
                u2.exec(doc.id, x.id);
            }
        }

        void    field_notify(Fragment frag, Change chg)
        {
        }

        void    field_notify_icons(Fragment frag, Change)
        {
            field_icon(cdb::field(cdb::document(frag), true));
        }

        void    field_stage3(Document doc)
        {
            std::string k   = cdb::skeyc(doc);
            if(k.empty())   //  skipping, no key
                return ;
            
            if(k.empty())
                return;

            std::string_view    ck  = k;
            Class               cls;
            size_t  n   = ck.find('.');
            bool            anycls  = true;
            if(n != std::string_view::npos){
                cls     = cdb::db_class(ck.substr(0,n));
                ck      = ck.substr(n+1);
                anycls  = false;
            }
            
            
            //  For the data, do a manual merge
            auto dp     = std::make_shared<Field::Data>();
            for(Fragment f : cdb::fragments(doc, DataRole::Config)){
                Field::SharedFile    p   = field_doc(f, cdb::DONT_LOCK);
                if(!p)
                    continue;
                cdb::update(f);
                dp->merge(*p, false);
            }

            Category    cat;
            if(!dp->category.empty())
                cat = cdb::db_category(dp->category);

            Image       img     = cdb::best_image(doc);

            static thread_local cdb::SQ iField("INSERT INTO FIELDS (id,k,class,ck,expected,icon,name,plural,pkey,brief,multi,restrict,category,maxcnt,anycls) VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
                
            iField.bind(1, doc.id);
            iField.bind(2, k);
            iField.bind(3, cls.id);
            iField.bind(4, ck);
            iField.bind(5, dp->expected);
            iField.bind(6, img.id);
            iField.bind(7, dp->name);
            iField.bind(8, dp->plural);
            iField.bind(9, dp->pkey);
            iField.bind(10, dp->brief);
            iField.bind(11, (int) dp->multiplicity);
            iField.bind(12, (int) dp->restriction);
            iField.bind(13, cat.id);
            iField.bind(14, dp->max_count);
            iField.bind(15, anycls);
            iField.exec();
            iField.reset();
            
            Field   x   = { doc.id };
            
            static thread_local cdb::SQ iTag("INSERT INTO FTags (field, tag) VALUES (?,?)");
            for(Tag t : cdb::tags_set(dp->tags, true))
                iTag.exec(x.id, t.id);

            static thread_local cdb::SQ iAlias("INSERT INTO FAlias (field, alias) VALUES (?,?)");
            for(std::string_view s : dp->aliases)
                iAlias.exec(x.id, s);

            static thread_local cdb::SQ iDataType("INSERT INTO FDataTypes (field, type) VALUES (?,?)");
            for(std::string_view s : dp->types){
                const TypeInfo* ti  = TypeInfo::find(s);
                if(!ti){
                    yInfo() << "Invalid data type '" << s << "' specified in field " << k;
                    continue;
                }
                
                iDataType.exec(x.id, ti->id());
            }
            
            static thread_local cdb::SQ iAtomType("INSERT INTO FAtomTypes (field, class, hops) VALUES (?, ?, ?)");
            auto cset   = make_set(cdb::db_classes(dp->atoms));
            for(auto itr : cdb::derived_classes_ranked_merged_map(cset, true)){
                iAtomType.bind(1, x.id);
                iAtomType.bind(2, itr.first.id);
                iAtomType.bind(3, itr.second.cnt);
                iAtomType.exec();
            }

            static thread_local cdb::SQ iClass0("INSERT INTO CFields (class, field, hops) VALUES (?, ?, 0)");
            iClass0.exec(cls.id, x.id);
            
            static thread_local cdb::SQ iClassA("INSERT INTO CFields (class, field, hops) VALUES (?, ?, -1)");
            static thread_local cdb::SQ iClass( "INSERT INTO CFields (class, field, hops) VALUES (?, ?, ?)");
            if(anycls){
                for(Class c : cdb::all_classes())
                    iClassA.exec(c.id, x.id);
            } else {
                for(auto& itr : cdb::derived_classes_ranked(cls)){
                    iClass.bind(1, itr.cls.id);
                    iClass.bind(2, x.id);
                    iClass.bind(3, 1+itr.rank);
                    iClass.exec();
                }
            }
        }
    }
}
