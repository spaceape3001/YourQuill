////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

 #pragma once
 
 namespace {
 
    void    remove_tag(Tag x)
    {
        static thread_local SQ  stmts[] = {
            SQ( "DELETE FROM CTags WHERE tag=?" ),
            SQ( "DELETE FROM Tags WHERE id=?" )
        };
        for(auto& sq : stmts)
            sq.exec(x.id);
    }
    
    void    update_tag(Tag x)
    {
        Tag::SharedData data = merged(x, IS_UPDATE);
        if(!data)
            return ;
            
        Leaf    l;  // ignore for now
        
        static thread_local SQ u1("UPDATE Tags SET name=?,brief=?,leaf=? WHERE Tags=?");
        auto af = u1.af();
        u1.bind(1, data->name);
        u1.bind(2, data->brief);
        u1.bind(3, l.id);
        u1.bind(4, x.id);
        u1.exec();
    }
 
    void    change_tag(Fragment frag, Change chg)
    {
        Document    doc = document(frag);
        Tag         t   = db_tag(doc);
        if(!t)
            return ;

        if(chg == Change::Removed){
            if(fragments_count(doc) <= 1){
                remove_tag(t);
                return ;
            }
        }
        
        update_tag(t);
    }
 
    void    change_tag_image(Fragment frag, Change)
    {
        Document    doc = document(frag);
        std::string bk  = base_key(doc);
        if(bk.empty())
            return;
        bk += ".tag";
        doc             = child_document(tags_folder(), bk);
        if(!doc)
            return ;
            
        Tag     x   = tag(doc);
        if(x){
            Image   img0    = icon(x);
            Image   img1    = best_image(doc);
            if(img0 == img1)
                return ;
            
            static thread_local SQ u1("UPDATE Tags SET icon=? WHERE id=?");
            auto af = u1.af();
            u1.bind(1, img1.id);
            u1.bind(2, x.id);
            u1.exec();
        }
    }
 
    //  TODO images & leaf....
 
    YQ_INVOKE( 
        u_change<change_tag>(tags_folder(), "*.tag"); 
        for(const char* z : Image::kSupportedExtensionWildcards)
            u_change<change_tag_image>(tags_folder(), z);
    )
 }
 
