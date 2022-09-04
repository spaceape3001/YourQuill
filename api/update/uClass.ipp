////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uClass.hpp"

namespace yq {
    UClass&  uget(Class a)
    {
        static std::map<uint64_t, UClass*>  s_data;
        UClass*& p  = s_data[a.id];
        if(!p)
            p       = new UClass(a);
        return *p;
    }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    void    UClass::s3_create(Document doc)
    {
        uget(db_class(doc));
    }

    void    UClass::s3_initialize(Document doc)
    {
        Class   x = db_class(doc);
        UClass& u = uget(x);
        
        if(!u.reload(DONT_LOCK))
            return ;
        u.u_header();
    }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    UClass::UClass(Class a) : k(cdb::key(a)), cls(a) 
    {
    }

    bool    UClass::reload()
    {
        auto nd     = merged(cls, DONT_LOCK|IS_UPDATE);
        if(!nd){
            yWarning() << "Class '" << k << "' has no data!";
            return false;
        }
        data    = nd;
        return true;
    }

    void    UClass::u_header()
    {
        static thread_local SQ uInfo("UPDATE Classes SET name=?, plural=?, brief=? WHERE id=?");
        uInfo.bind(1, data->name);
        uInfo.bind(2, data->plural);
        uInfo.bind(3, data->brief);
        uInfo.bind(4, id);
        uInfo.exec();
    }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    Class::SharedData   update(Class x, cdb_options_t opts)
    {
        if(!x)
            return Class::SharedData{};

        if(opts & U_ICON)
            update_icon(x);
        
        Class::SharedData data = merged(x, opts);
        if(!data)
            return Class::SharedData();
        
        std::string     k   = key(x);

        static thread_local SQ iAlias("INSERT INTO CAlias (class, alias) VALUES (?,?)");
        static thread_local SQ iPrefix("INSERT INTO CPrefix (class, prefix) VALUES (?,?)");
        static thread_local SQ iSuffix("INSERT INTO CSuffix (class, suffix) VALUES (?,?)");
        static thread_local SQ iLookup("INSERT INTO CLookup (class, k, priority) VALUES (?,?,0)");
        static thread_local SQ iTag("INSERT INTO CTags (class, tag) VALUES (?,?)");
        
        static thread_local SQ dAlias("DELETE FROM CAlias WHERE class=? AND alias=?");
        static thread_local SQ dPrefix("DELETE FROM CPrefix WHERE class=? AND prefix=?");
        static thread_local SQ dSuffix("DELETE FROM CSuffix WHERE class=? AND suffix=?");
        static thread_local SQ dLookup("DELETE FROM CLookup WHERE class=? AND k=?");
        static thread_local SQ dTag("DELETE FROM CTags WHERE class=? AND tag=?");

        static thread_local SQ uInfo("UPDATE Classes SET name=?, plural=?, brief=?, category=?, binding=? WHERE id=?");

        if(opts & U_INFO){
            Category cat = category(data->category);
            uInfo.bind(1, data->name);
            uInfo.bind(2, data->plural);
            uInfo.bind(3, data->brief);
            uInfo.bind(4, cat.id);
            uInfo.bind(5, data->binding);
            uInfo.bind(6, x.id);
            uInfo.exec();
            
            string_set_t    old_aliases         = def_aliases(x);
            string_set_t    old_prefixes        = def_prefixes(x);
            string_set_t    old_suffixes        = def_suffixes(x);
            string_set_t    old_alternatives    = alternative_keys(x);
            
            string_set_t    alts;
            for(auto& p : data->prefixes){
                alts << p + k;
                for(auto& a : data->aliases){
                    alts << p + a;
                    for(auto & s : data->suffixes)
                        alts << p + a + s;
               }
            }
            
            for(auto& a : data->aliases){
                alts << a;
                for(auto& s : data->suffixes)
                    alts << a + s;
            }
            
            for(auto& s : data->suffixes)
                alts << k + s;
            
            auto    ch_alias        = add_remove(old_aliases, data->aliases);
            auto    ch_prefix       = add_remove(old_prefixes, data->prefixes);
            auto    ch_suffix       = add_remove(old_suffixes, data->suffixes);
            auto    ch_alternative  = add_remove(old_alternatives, alts);
            
            iAlias.batch(x.id, ch_alias.added);
            dAlias.batch(x.id, ch_alias.removed);
            
            iPrefix.batch(x.id, ch_prefix.added);
            dPrefix.batch(x.id, ch_prefix.removed);

            iSuffix.batch(x.id, ch_suffix.added);
            dSuffix.batch(x.id, ch_suffix.removed);

            iLookup.batch(x.id, ch_alternative.added);
            dLookup.batch(x.id, ch_alternative.removed);
        }
        
        if(opts & U_TAGS){
            std::set<Tag>   old_tags = make_set(tags(x));
            std::set<Tag>   new_tags = tags_set(data->tags, true);
            
            auto ch_tag = add_remove(old_tags, new_tags);
            iTag.batch(x.id, ids_for(ch_tag.added));
            dTag.batch(x.id, ids_for(ch_tag.removed));
        }
        
        return data;
    }
    
    
    void                update_icon(Class x)
    {
        if(!x)
            return ;
            
        static thread_local SQ u1("UPDATE Classes SET icon=? WHERE id=?");
        static thread_local SQ u2("UPDATE Documents SET icon=? WHERE id=?");

        Document    doc     = document(x);
        Image       img     = best_image(doc);
        if(img != icon(x)){
            u1.exec(img.id, x.id);
            u2.exec(doc.id, x.id);
        }
    }

}

