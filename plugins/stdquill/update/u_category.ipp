////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

 #pragma once
 
 namespace {
 
    void    remove_category(Category x)
    {
        static thread_local SQ  stmts[] = {
            SQ( "UPDATE Classes SET category=0 WHERE category=?" ),
            SQ( "DELETE FROM Categories WHERE id=?" )
        };
        for(auto& sq : stmts)
            sq.exec(x.id);
    }
    
    void    update_category(Category x)
    {
        Category::SharedData data = merged(x, IS_UPDATE);
        if(!data)
            return ;
            
        Leaf    l;  // ignore for now
        
        static thread_local SQ u1("UPDATE Categories SET name=?,brief=? WHERE Categorys=?");
        auto af = u1.af();
        u1.bind(1, data->name);
        u1.bind(2, data->brief);
        u1.bind(3, x.id);
        u1.exec();
    }
 
    void    change_category(Fragment frag, Change chg)
    {
        Document    doc = document(frag);
        Category         t   = db_category(doc);
        if(!t)
            return ;

        if(chg == Change::Removed){
            if(fragments_count(doc) <= 1){
                remove_category(t);
                return ;
            }
        }
        
        update_category(t);
    }
 
    void    change_category_image(Fragment frag, Change)
    {
        Document    doc = document(frag);
        std::string bk  = base_key(doc);
        if(bk.empty())
            return;
        bk += ".cat";
        doc             = child_document(categories_folder(), bk);
        if(!doc)
            return ;
            
        Category     x   = category(doc);
        if(x){
            Image   img0    = icon(x);
            Image   img1    = best_image(doc);
            if(img0 == img1)
                return ;
            
            static thread_local SQ u1("UPDATE Categories SET icon=? WHERE id=?");
            auto af = u1.af();
            u1.bind(1, img1.id);
            u1.bind(2, x.id);
            u1.exec();
        }
    }
 
    //  TODO images & leaf....
 
    YQ_INVOKE( 
        u_change<change_category>(categories_folder(), "*.cat"); 
        for(const char* z : Image::kSupportedExtensionWildcards)
            u_change<change_category_image>(categories_folder(), z);
    )
 }
 
