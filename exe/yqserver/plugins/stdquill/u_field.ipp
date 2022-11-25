////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once


#include "u_field.hpp"
#include <basic/CollectionUtils.hpp>
#include <basic/Logging.hpp>
#include <basic/meta/TypeInfo.hpp>
#include <kernel/atom/ClassCDB.hpp>
#include <kernel/atom/FieldCDB.hpp>
#include <kernel/atom/FieldData.hpp>
#include <kernel/db/SQ.hpp>
#include <kernel/file/DocumentCDB.hpp>
#include <kernel/file/FragmentCDB.hpp>
#include <kernel/image/ImageCDB.hpp>
#include <kernel/org/CategoryCDB.hpp>
#include <kernel/org/TagCDB.hpp>

#if 0
namespace {
        // called POST class-expansion
    void    field_s3_classes(Document doc)
    {
        Field               x   = db_field(doc);
        Field::SharedData data  = merged(x, DONT_LOCK);
        if(!data){
            yWarning() << "Unable to initialize field '" << key(x) << "'";
            return ;
        }
        
        static thread_local SQ iAtoms("INSERT INTO FAtomTypes (field, class) VALUES (?,?)");
        iAtoms.batch(x.id, ids_for(classes_set(data->atoms, true)));
    }

        // called BEFORE class-expansion
    void    field_s3_init(Document doc)
    {
        Field            x   = db_field(doc);
        Field::SharedData data   = update_this(x, DONT_LOCK|U_INFO|U_ICON|U_TAGS);
        if(!data){
            yWarning() << "Unable to initialize field '" << key(x) << "'";
            return ;
        }
        
        static thread_local SQ iClass("INSERT INTO CFields (field, class, hops) VALUES (?, ?, 0)");
        
        Class   c = class_(x);
        if(c){
            iClass.exec(x.id, c.id);
        } else
            iClass.batch(x.id, ids_for(classes_set(data->classes, true)));
    }
    
    void    field_update(Fragment frag, Change chg)
    {
        Document    doc = document(frag);
        if(chg == Change::Removed){
            if(fragments_count(doc) <= 1){
                update::field_erase(doc);
                return ;
            }
        }

        bool        created = false;
        Field       x   = db_field(doc, &created);
        if(!x)
            return ;
            
        cdb_options_t       opts    = U_INFO|U_TAGS|IS_UPDATE;
        if(created)
            opts |= U_ICON;
            
        Field::SharedData   data    = update_this(x, opts);
        if(!data){
            yWarning() << "Unable to update field '" << key(x) << "'";
            return ;
        }
    }
}
#endif

namespace yq {
    void    u_field_erase(Field x)
    {
        static thread_local SQ stmts[] = {
            SQ("DELETE FROM CFields WHERE field=?"),
            SQ("DELETE FROM FTags WHERE field=?"),
            SQ("DELETE FROM Fields WHERE id=?")
        };

        if(!x)
            return ;
        
        for(auto& sq : stmts)
            sq.exec(x.id);
    }

    void    u_field_erase(Document doc)
    {
        u_field_erase(field(doc));
    }

    void    u_field_icon(Field x)
    {
        if(!x)
            return ;
            
        Document    doc     = document(x);
        Image       img     = best_image(doc);
        
        static thread_local SQ u1("UPDATE Fields SET icon=? WHERE id=?");
        static thread_local SQ u2("UPDATE Documents SET icon=? WHERE id=?");

        if(icon(x) != img){
            u1.exec(img.id, x.id);
            u2.exec(doc.id, x.id);
        }
    }

    void    u_field_notify(Fragment frag, Change chg)
    {
    }

    void    u_field_notify_icons(Fragment frag, Change)
    {
        u_field_icon(field(document(frag), true));
    }

    void    u_field_stage3(Document doc)
    {
        std::string k   = skeyc(doc);
        if(k.empty())   //  skipping, no key
            return ;
        
        if(k.empty())
            return;

        std::string_view    ck  = k;
        Class               cls;
        size_t  n   = ck.find('.');
        bool            anycls  = true;
        if(n != std::string_view::npos){
            cls     = db_class(ck.substr(0,n));
            ck      = ck.substr(n+1);
            anycls  = false;
        }
        
        
        //  For the data, do a manual merge
        auto dp     = std::make_shared<Field::Data>();
        for(Fragment f : fragments(doc, DataRole::Config)){
            Field::SharedFile    p   = field_doc(f, DONT_LOCK);
            if(!p)
                continue;
            update(f);
            dp->merge(*p, false);
        }

        Category    cat;
        if(!dp->category.empty())
            cat = db_category(dp->category);

        Image       img     = best_image(doc);

        static thread_local SQ iField("INSERT INTO FIELDS (id,k,class,ck,expected,icon,name,plural,pkey,brief,multi,restrict,category,maxcnt,anycls) VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
            
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
        
        static thread_local SQ iTag("INSERT INTO FTags (field, tag) VALUES (?,?)");
        for(Tag t : find_tags_set(dp->tags, true))
            iTag.exec(x.id, t.id);

        static thread_local SQ iAlias("INSERT INTO FAlias (field, alias) VALUES (?,?)");
        for(std::string_view s : dp->aliases)
            iAlias.exec(x.id, s);

        static thread_local SQ iDataType("INSERT INTO FDataTypes (field, type) VALUES (?,?)");
        for(std::string_view s : dp->types){
            const TypeInfo* ti  = TypeInfo::find(s);
            if(!ti){
                yInfo() << "Invalid data type '" << s << "' specified in field " << k;
                continue;
            }
            
            iDataType.exec(x.id, ti->id());
        }
        
        static thread_local SQ iAtomType("INSERT INTO FAtomTypes (field, class, hops) VALUES (?, ?, ?)");
        auto cset   = make_set(db_classes(dp->atoms));
        for(auto itr : derived_classes_ranked_merged_map(cset, true)){
            iAtomType.bind(1, x.id);
            iAtomType.bind(2, itr.first.id);
            iAtomType.bind(3, itr.second.cnt);
            iAtomType.exec();
        }

        static thread_local SQ iClass0("INSERT INTO CFields (class, field, hops) VALUES (?, ?, 0)");
        iClass0.exec(cls.id, x.id);
        
        static thread_local SQ iClassA("INSERT INTO CFields (class, field, hops) VALUES (?, ?, -1)");
        static thread_local SQ iClass( "INSERT INTO CFields (class, field, hops) VALUES (?, ?, ?)");
        if(anycls){
            for(Class c : all_classes())
                iClassA.exec(c.id, x.id);
        } else {
            for(auto& itr : derived_classes_ranked(cls)){
                iClass.bind(1, itr.cls.id);
                iClass.bind(2, x.id);
                iClass.bind(3, 1+itr.rank);
                iClass.exec();
            }
        }
    }
}
