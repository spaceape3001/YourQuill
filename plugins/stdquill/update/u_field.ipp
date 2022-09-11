////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

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
        Field::SharedData data   = update(x, DONT_LOCK|U_INFO|U_ICON|U_TAGS);
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
    
    void    field_icons(Fragment frag)
    {
        update_icon(field(document(frag), true));
    }
    
    void    field_update(Fragment frag, Change chg)
    {
        Document    doc = document(frag);
        if(chg == Change::Removed){
            if(fragments_count(doc) <= 1){
                erase(field(doc));
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
            
        Field::SharedData   data    = update(x, opts);
        if(!data){
            yWarning() << "Unable to update field '" << key(x) << "'";
            return ;
        }
    }
}
