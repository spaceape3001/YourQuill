////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    
    void    s3_tag_leaf(Document doc)
    {
        Tag     x       = db_tag(doc);
        
        Tag::SharedData data = merged(x, DONT_LOCK);
        if(!data)
            return ;
        
        Leaf    l;  // TODO
        
        if(!l)
            return ;
        
        static thread_local SQ u("UPDATE Tags SET leaf=? WHERE id=?");
        auto af = u.af();
        u.bind(1, l.id);
        u.bind(2, x.id);
        u.exec();
    }
    
    YQ_INVOKE( )
    
}
