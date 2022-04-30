////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    void    s3_leaf(Document doc)
    {
        Leaf    l   = db_leaf(doc);
        auto data  = merged(l, DONT_LOCK|IS_UPDATE);
        if(!data)
            return ;
        
        Image   img = best_image(doc);
        
        std::string_view title   = data->title();
        std::string_view abbr    = data->abbr();
        std::string_view brief   = data->brief();
        
        static thread_local SQ u("UPDATE Leafs SET icon=?, title=?, abbr=?, brief=? WHERE id=?");
        u.bind(1, img.id);
        if(title.empty()){
            u.bind(2, key(l));  // fall back (for now) 
                                // TODO ... make this smarter (later)
        } else 
            u.bind(2, title);
        u.bind(3, abbr);
        u.bind(4, brief);
        u.bind(5, l.id);
        u.exec();

        // do the rest...later

    }

    
    YQ_INVOKE( 
        u_stage3<s3_leaf>("*.y");
    )
    
}
