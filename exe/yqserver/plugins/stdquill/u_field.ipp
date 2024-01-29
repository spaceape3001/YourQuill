////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void    u_field(Field f, Change chg)
    {
        Field::Diff   x { 
            .x      = f, 
            .chg    = chg, 
            .key    = cdb::key(f),
            .subkey = cdb::skey(f),
            .cls    = cdb::class_(f),
            .anycls = cdb::is_any(f)
        };
        
        if(chg != Change::Added){
            Field::Info  ii         = cdb::info(f);
            
            x.brief.from            = std::move(ii.brief);
            x.category.from         = ii.category;
            x.expected.from         = std::move(ii.expected);
            x.icon.from             = ii.icon;
            x.max_count.from        = ii.max_count;
            x.multiplicity.from     = ii.multiplicity;
            x.name.from             = std::move(ii.name);
            x.pkey.from             = std::move(ii.pkey);
            x.plural.from           = std::move(ii.plural);
            x.restriction.from      = ii.restriction;
            
            x.aliases.from          = cdb::aliases(f);
            x.tags.from             = cdb::tags_set(f);
        }

        static thread_local CacheQuery uInfo("UPDATE Fields SET name=?, plural=?, pkey=?, expected=?, brief=?, multi=?, restrict=?, category=?, maxcnt=?, icon=? WHERE id=?");

        Document doc    = cdb::document(f);
        if(chg != Change::Removed){
        
            //  Not removed, so update the field information, aliases, etc
            
            auto def        = cdb::merged(f, cdb::DONT_LOCK|cdb::IS_UPDATE);
            if(!def){
                yInfo() << "Field " << x.key << " cannot be read.";
                return;
            }
            
            x.brief.to              = std::move(def->brief);
            if(!def->category.empty())
                x.category.to       = cdb::db_category(def->category);
            x.expected.to           = std::move(def->expected);
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
            
        }

        if(chg == Change::Removed){
            x.tags.removed          = x.tags.from;
            x.aliases.removed       = x.aliases.from;
        }
    }
    
    void    s3_field(Document doc)
    {
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
