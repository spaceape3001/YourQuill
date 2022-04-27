////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    

    void    remove_field(Document doc)
    {   
        Field fld   = field(doc);
        if(!fld)
            return ;
            
        static thread_local SQ stmts[] = {
            SQ("DELETE FROM FDefClass WHERE field=?"),
            SQ("DELETE FROM FTags WHERE field=?"),
            SQ("DELETE FROM Fieldes WHERE id=?")
        };
        
        for(auto& sq : stmts)
            sq.exec(fld.id);
    }

    void    update_field(Document doc)
    {
        Field               x;
        Field::SharedData   data;
        std::tie(x,data) = update_field_info(doc);
        if(!x)
            return ;

        //  Now it's time for tags/etc
    }
    
    void    change_field(Fragment frag, Change chg)
    {
        Document    doc = document(frag);
        if(chg == Change::Removed){
            if(fragments_count(doc) <= 1){
                remove_field(doc);
                return ;
            }
        }
        update_field(doc);
    }
    
    void    change_field_image(Fragment frag, Change)
    {
        Document    doc = document(frag);
        std::string bk  = skeyc(doc);
        if(bk.empty())
            return;
        bk += ".fld";
        doc             = child_document(fields_folder(), bk);
        if(!doc)
            return ;
            
        Field    x   = field(doc);
        if(x){
            Image   img0    = icon(x);
            Image   img1    = best_image(doc);
            if(img0 == img1)
                return ;
            
            static thread_local SQ u1("UPDATE Field SET icon=? WHERE id=?");
            auto af = u1.af();
            u1.bind(1, img1.id);
            u1.bind(2, x.id);
            u1.exec();
        }
    }    
    
    YQ_INVOKE( 
        u_change<change_field>(fields_folder(), "*.fld");
        for(const char* z : Image::kSupportedExtensionWildcards)
            u_change<change_field_image>(fields_folder(), z);
        
    )
    
}
