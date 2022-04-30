////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    void    s3_leaf(Document doc)
    {
        Leaf            l   = db_leaf(doc);
        Leaf::SharedData data   = update_info(l, DONT_LOCK);
        if(!data)
            return ;
        update_icon(l);
        auto rep        = diff::additions(doc, data->attrs);
        
        
        rep.exec_inserts();

        // do the rest...later

    }

    
    YQ_INVOKE( 
        u_stage3<s3_leaf>("*.y");
    )
    
}
