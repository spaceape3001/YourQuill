////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/field/Field.hpp>
#include <mithril/field/FieldCDB.hpp>
#include <mithril/field/FieldData.hpp>
#include <mithril/field/FieldDiff.hpp>
#include <mithril/field/FieldInfo.hpp>

namespace {

    using MetaId = yq::mithril::Meta;

    void    u_field(Field f, Change chg, bool s3=false)
    {
        if(!f)
            return ;
    
        Field::Diff   x { 
            .x      = f, 
            .id     = f.id,
            .chg    = chg, 
            .key    = cdb::key(f),
            .subkey = cdb::skey(f),
            .cls    = cdb::class_(f),
            .anycls = cdb::is_any(f)
        };
        
        static thread_local CacheQuery uInfo("UPDATE " TBL_FIELDS " SET name=?, plural=?, pkey=?, expected=?, brief=?, multi=?, restrict=?, category=?, maxcnt=?, icon=? WHERE id=?");
        static thread_local CacheQuery iAlias("INSERT INTO FAlias (field, alias, usurp) VALUES (?,?,0)");
        static thread_local CacheQuery dAlias("DELETE FROM FAlias WHERE field=? AND alias=? AND usurp=0");
        static thread_local CacheQuery sAlias("SELECT alias FROM FAlias WHERE field=? AND usurp=0");
        static thread_local CacheQuery sUsurp("SELECT alias FROM FAlias WHERE field=? AND usurp=1");
        static thread_local CacheQuery iUsurp("INSERT INTO FAlias (field, alias, usurp) VALUES (?,?,1)");
        static thread_local CacheQuery dUsurp("DELETE FROM FAlias WHERE field=? AND alias=? AND usurp=1");
        static thread_local CacheQuery iTag("INSERT INTO " TBL_FIELD_TAG " (field, tag) VALUES (?,?)");
        static thread_local CacheQuery dTag("DELETE FROM " TBL_FIELD_TAG " WHERE field=? AND tag=?");
        static thread_local CacheQuery iType("INSERT INTO FTypes (field, type) VALUES (?,?)");
        static thread_local CacheQuery dType("DELETE FROM FTypes WHERE field=? AND type=?");

        if(chg != Change::Added){
            Field::Info  ii         = cdb::info(f);
            
            x.brief.from            = std::move(ii.brief);
            x.category.from         = ii.category;
            x.expected.from         = ii.expected;
            x.icon.from             = ii.icon;
            x.max_count.from        = ii.max_count;
            x.multiplicity.from     = ii.multiplicity;
            x.name.from             = std::move(ii.name);
            x.pkey.from             = std::move(ii.pkey);
            x.plural.from           = std::move(ii.plural);
            x.restriction.from      = ii.restriction;
            
            x.aliases.from          = sAlias.sset(f.id);
            x.usurps.from           = sUsurp.sset(f.id);
            x.types.from            = make_set(cdb::types(f));
            x.tags.from             = cdb::tags_set(f);
        }


        Document doc    = cdb::document(f);
        if(chg != Change::Removed){
        
            //  Not removed, so update the field information, aliases, etc
            
            auto def        = cdb::merged(f, cdb::DONT_LOCK|cdb::IS_UPDATE);
            if(!def){
                yInfo() << "Field " << x.key << " cannot be read.";
                return;
            }
            
            for(auto & s : def->types){
                bool    fSet    = is_similar(s, def->expected);
                
                auto decl = [&](Id i) {
                    if(!i)
                        return ;
                    if(fSet)
                        x.expected.to   = i;
                    x.types.to.insert(i);
                };
            
                Class   fc  = cdb::class_(s);
                if(fc)
                    decl(fc);
                for(Class c2 : cdb::derived_classes(fc))
                    x.types.to.insert(c2);
                
                const TypeMeta* ti  = TypeMeta::find(s);
                if(ti)
                    decl(MetaId(ti->id()));
            }
            
            
            
            x.aliases.to            = std::move(def->aliases);
            x.brief.to              = std::move(def->brief);
            if(!def->category.empty())
                x.category.to       = cdb::db_category(def->category);
            x.icon.to               = cdb::best_image(doc);
            x.max_count.to          = def->max_count;
            x.multiplicity.to       = def->multiplicity;
            
            if(!def->name.empty()){
                x.name.to           = std::move(def->name);
            } else {
                x.name.to           = x.subkey;
            }
            
            if(!def->pkey.empty()){
                x.pkey.to           = std::move(def->pkey);
            } else {
                x.pkey.to           = to_lower(pluralize(x.subkey));
            }
            
            if(!def->plural.empty()){
                x.plural.to         = std::move(def->plural);
            } else {
                x.plural.to         = pluralize(x.name.to);
            }
            
            x.restriction.to        = def->restriction;
            x.usurps.to             = std::move(def->usurps);
            
            uInfo.bind(1, x.name.to);
            uInfo.bind(2, x.plural.to);
            uInfo.bind(3, x.pkey.to);
            uInfo.bind(4, x.expected.to);
            uInfo.bind(5, x.brief.to);
            uInfo.bind(6, (int) x.multiplicity.to);
            uInfo.bind(7, (int) x.restriction.to);
            uInfo.bind(8, x.category.to.id);
            uInfo.bind(9, x.max_count.to);
            uInfo.bind(10, x.icon.to.id);
            uInfo.bind(11, f.id);
            
            uInfo.exec();
            
            //  Aliases
            x.aliases.analyze();
            for(const std::string& s : x.aliases.added)
                iAlias.exec(x.id, s);
                
            
            // Types
            x.types.analyze();
            for(Id u : x.types.added)
                iType.exec(x.id, u.m_value);

            // Tags
            x.tags.to           = cdb::find_tags_set(def->tags, true);
            x.tags.analyze();
            
            for(Tag t : x.tags.added)
                iTag.exec(x.id, t.id);
            
            
            //  Usurps
            x.usurps.analyze();
            for(const std::string& s : x.usurps.added)
                iUsurp.exec(f.id, s);
        }

        if(chg == Change::Removed){
            x.aliases.removed       = x.aliases.from;
            x.types.removed         = x.types.from;
            x.classes.removed       = x.classes.from;
            x.tags.removed          = x.tags.from;
            x.usurps.removed        = x.usurps.from;
        }
        
        Field::Notify::notify(x);
        
        if(chg != Change::Removed){
            for(const std::string& s : x.aliases.removed)
                dAlias.exec(f.id, s);
            for(Id u : x.types.removed)
                dType.exec(f.id, u.m_value);
            for(Tag t : x.tags.removed)
                dTag.exec(f.id, t.id);
            for(const std::string& s : x.usurps.removed)
                dUsurp.exec(f.id, s);
        } else {
            static thread_local CacheQuery stmts[] = {
                CacheQuery("DELETE FROM " TBL_CLASS_FIELD " WHERE field=?"),
                CacheQuery("DELETE FROM " TBL_FIELD_TAG " WHERE field=?"),
                CacheQuery("DELETE FROM FAlias WHERE field=?"),
                CacheQuery("DELETE FROM FTypes WHERE field=?"),
                CacheQuery("DELETE FROM " TBL_FIELDS " WHERE id=?")
            };
            for(auto& sq : stmts)
                sq.exec(f.id);
        }
    }
    
    void    s3_field(Document doc)
    {
        u_field(cdb::db_field(doc), Change::Added, true);
    }
    
    void    s5_field(Fragment frag, Change chg)
    {
        Document    doc = cdb::document(frag);
        if(chg == Change::Removed){
            if(cdb::fragments_count(doc) <= 1){
                Field x = cdb::field(doc);
                if(x){
                    u_field(x, Change::Removed);
                }
                return ;
            }
            
            chg = Change::Modified;
        }
        
        bool        created     = false;
        Field x       = cdb::db_field(doc, &created);
        if(created){
            u_field(x, Change::Added);
        } else
            u_field(x, Change::Modified);
    }

    void    s5_field_icons(Fragment frag, Change chg)
    {
        Field    x   = cdb::field(cdb::document(frag), true);
        if(!x)
            return ;
        u_field(x, Change::Modified);
    }
}
