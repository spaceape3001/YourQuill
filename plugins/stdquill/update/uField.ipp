////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uField.hpp"
#include <basic/Vector.hpp>
#include <basic/meta/TypeInfo.hpp>
#include <kernel/db/SQ.hpp>
#include <kernel/atom/FieldCDB.hpp>

namespace yq {
    UField&  uget(Field a)
    {
        static std::map<uint64_t, UField*>   s_data;
        UField*& p  = s_data[a.id];
        if(!p)
            p       = new UField(a);
        return *p;
    }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        //! Creates the database-lookups only
    void    UField::s3_create(Document doc)
    {
        uget(db_field(doc));
    }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    UField::UField(Field a) : k(cdb::key(a)), field(a) 
    {
    }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    Field::SharedData       update_this(Field x, cdb_options_t opts)
    {
        if(!x)
            return Field::SharedData();
            
        if(opts & U_ICON)
            update_icon(x);

        Field::SharedData data = merged(x, IS_UPDATE | opts);
        if(!data)
            return Field::SharedData();
        
        static thread_local SQ iAlias("INSERT INTO FAlias (field, alias) VALUES (?,?)");
        static thread_local SQ dAlias("DELETE FROM FAlias WHERE field=? AND alias=?");
        static thread_local SQ iTypes("INSERT INTO FDataTypes (field, type) VALUES (?,?)");
        static thread_local SQ dTypes("DELETE FROM FDataTypes WHERE field=? AND type=?");
        static thread_local SQ iTags("INSERT INTO FTags (field, tag) VALUES (?, ?)");
        static thread_local SQ dTags("DELETE FROM FTags WHERE field=? AND tag=?");
        static thread_local SQ uInfo("UPDATE Fields SET name=?,brief=?,multi=?,restrict=?,category=?,pkey=?,expected=?,maxcnt=?,plural=? WHERE id=?");
        
        if(opts & U_INFO){
            Category cat = category(data->category);

            uInfo.bind(1, data->name);
            uInfo.bind(2, data->brief);
            uInfo.bind(3, data->multiplicity.value());
            uInfo.bind(4, data->restriction.value());
            uInfo.bind(5, cat.id);
            uInfo.bind(6, data->pkey);
            uInfo.bind(7, data->expected);
            uInfo.bind(8, data->max_count);
            uInfo.bind(9, data->plural);
            uInfo.bind(10, x.id);
            uInfo.exec();
            
            
            string_set_t    old_aliases = aliases(x);
            std::set<uint64_t> old_types = data_types(x);
            std::set<uint64_t> new_types = ids_for(TypeInfo::find_all(data->types, true));
            
            auto ch_aliases = add_remove(old_aliases, data->aliases);
            auto ch_types   = add_remove(old_types, new_types);
            
            iAlias.batch(x.id, ch_aliases.added);
            dAlias.batch(x.id, ch_aliases.removed);
            iTypes.batch(x.id, ch_types.added);
            dTypes.batch(x.id, ch_types.removed);
        }
        
        if(opts & U_TAGS){
            std::set<Tag>       old_tags    = tags_set(x);
            std::set<Tag>       new_tags    = tags_set(data->tags, true);
            
            auto ch_tags    = add_remove(old_tags, new_tags);
            iTags.batch(x.id, ids_for(ch_tags.added));
            dTags.batch(x.id, ids_for(ch_tags.removed));
        }
        
        //  AND MORE TO GO.....
        
        return data;
    }
    
    void                update_icon(Field x)
    {
        if(!x)
            return ;
            
        Document    doc     = document(x);
        Image       img     = best_image(doc);
        
        static thread_local SQ u1("UPDATE Fields SET icon=? WHERE id=?");
        static thread_local SQ u2("UPDATE Documents SET icon=? WHERE id=?");

        if(icon(doc) != img){
            u1.exec(img.id, x.id);
            u2.exec(doc.id, x.id);
        }
    }
}

