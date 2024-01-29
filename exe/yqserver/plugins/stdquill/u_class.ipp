////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void    class_graph_redraw(bool trigger=false)
    {
        static bool     s_ready = false;
        
        if(!s_ready){
            if(!trigger)
                return ;
                
            s_ready = true;
        }
        
            
    }

    void    u_class(Class c, Change chg)
    {
        Class::Diff   x { 
            .x = c, 
            .chg = chg, 
            .key = cdb::key(c)
        };
        
        Document doc    = cdb::document(c);
        
        if(chg != Change::Added){
            Class::Info     ii  = cdb::info(c);
            x.category.from     = ii.category;
            x.icon.from         = ii.icon;
            x.name.from         = std::move(ii.name);
            x.plural.from       = std::move(ii.plural);
            x.brief.from        = std::move(ii.brief);
        }
        
        if(chg != Change::Removed){
            auto def        = cdb::merged(c, cdb::DONT_LOCK|cdb::IS_UPDATE);
            if(!def){
                yInfo() << "Class " << x.key << " cannot be read.";
                return;
            }

            x.icon.to           = cdb::best_image(doc);
            x.category.to       = cdb::category(def->category);
            x.name.to           = std::move(def->name);
            x.plural.to         = std::move(def->plural);
            x.brief.to          = std::move(def->brief);
            
            
        }
        
    }
    
        //  Classes go differently....
    
    void    s3_class(Document doc)
    {
        u_class(cdb::db_class(doc), Change::Added);
    }

    void    s3_class_info(Document doc)
    {
    }

    void    s3_class_bind(Document doc)
    {
    }

    void    s3_class_extend(Document doc)
    {
    }

    void    s3_class_derives(Document doc)
    {
    }

    void    s3_class_deduce(Document doc)
    {
    }

    void    s3_class_propagate(Document doc)
    {
    }
    
    void    s4_class()
    {
        class_graph_redraw();
        //  sends out notifications?
    }

    void    s5_class(Fragment frag, Change chg)
    {
        Document    doc = cdb::document(frag);
        if(chg == Change::Removed){
            if(cdb::fragments_count(doc) <= 1){
                Class x = cdb::class_(doc);
                if(x){
                    u_class(x, Change::Removed);
                }
                return ;
            }
            
            chg = Change::Modified;
        }
        
        bool        created     = false;
        Class x       = cdb::db_class(doc, &created);
        if(created){
            u_class(x, Change::Added);
        } else
            u_class(x, Change::Modified);
    }
    
    void    s5_class_icons(Fragment frag, Change chg)
    {
        Class    x   = cdb::class_(cdb::document(frag), true);
        if(!x)
            return ;
        u_class(x, Change::Modified);
    }


    //void    class_notify(Fragment frag,Change chg)
    //{
        //UClass::notify(frag, chg);
        //class_graph_redraw();
    //}
}
