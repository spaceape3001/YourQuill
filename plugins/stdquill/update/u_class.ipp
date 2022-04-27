////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    void    remove_class(Class cls)
    {
        if(!cls)
            return ;
            
        static thread_local SQ stmts[] = {
            SQ("DELETE FROM CAlias WHERE class=?"),
            SQ("DELETE FROM CPrefix WHERE class=?"),
            SQ("DELETE FROM CSuffix WHERE class=?"),
            SQ("DELETE FROM CTags WHERE class=?"),
            SQ("DELETE FROM Classes WHERE id=?")
        };
        
        for(auto& sq : stmts)
            sq.exec(cls.id);
    }

    void    update_class(Class cls)
    {
        Class::SharedData   data    = merged(cls, IS_UPDATE);
        Category    cat = category(data->category);
        
        static thread_local SQ  uc("UPDATE Classes SET name=?,plural=?,brief=?,category=?,binding=? WHERE id=?");
        uc.bind(1, data->name);
        uc.bind(2, data->plural);
        uc.bind(3, data->brief);
        uc.bind(4, cat.id);
        uc.bind(5, data->binding);
        uc.bind(6, cls.id);
        uc.exec();
        
        
        
    }
    
    void    change_class(Fragment frag, Change chg)
    {
        Document    doc = document(frag);
        Class       t   = db_class(doc);
        if(!t)
            return ;

        if(chg == Change::Removed){
            if(fragments_count(doc) <= 1){
                remove_class(t);
                return ;
            }
        }
        
        update_class(t);
    }
    
    void    change_class_image(Fragment frag, Change)
    {
        Document    doc = document(frag);
        std::string bk  = base_key(doc);
        if(bk.empty())
            return;
        bk += ".cls";
        doc             = child_document(classes_folder(), bk);
        if(!doc)
            return ;
            
        Class    x   = class_(doc);
        if(x){
            Image   img0    = icon(x);
            Image   img1    = best_image(doc);
            if(img0 == img1)
                return ;
            
            static thread_local SQ u1("UPDATE Classes SET icon=? WHERE id=?");
            auto af = u1.af();
            u1.bind(1, img1.id);
            u1.bind(2, x.id);
            u1.exec();
        }
    }    
    
    YQ_INVOKE( 
        u_change<change_class>(classes_folder(), "*.cls"); 
        for(const char* z : Image::kSupportedExtensionWildcards)
            u_change<change_class_image>(classes_folder(), z);
    )
    
}
