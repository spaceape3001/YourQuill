////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void    u_leaf(Leaf l, Change chg)
    {
        Leaf::Diff   x { 
            .x = l, 
            .chg = chg, 
            .key = cdb::key(l)
        };
        
        
    }
    
    void    s3_leaf(Document doc)
    {
        bool        created     = false;
        Leaf x       = cdb::db_leaf(doc, &created);
        u_leaf(x, Change::Added);
    }
    
    void    s5_leaf(Fragment frag, Change chg)
    {
        Document    doc = cdb::document(frag);
        if(chg == Change::Removed){
            if(cdb::fragments_count(doc) <= 1){
                Leaf x = cdb::leaf(doc);
                if(x){
                    u_leaf(x, Change::Removed);
                }
                return ;
            }
            
            chg = Change::Modified;
        }
        
        bool        created     = false;
        Leaf x       = cdb::db_leaf(doc, &created);
        if(created){
            u_leaf(x, Change::Added);
        } else
            u_leaf(x, Change::Modified);
    }

    void    s5_leaf_icons(Fragment frag, Change chg)
    {
        Leaf    x   = cdb::leaf(cdb::document(frag), true);
        if(!x)
            return ;
        u_leaf(x, Change::Modified);
    }
}
