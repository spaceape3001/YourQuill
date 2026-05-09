////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    bool    is_image(std::string_view sv)
    {
        return isImage(mimeTypeForExt(sv));
    }

    bool    u_doc_image(Document doc, Document img)
    {
        static thread_local CacheQuery s("SELECT id FROM Fragments WHERE document=?");
        static thread_local CacheQuery u("UPDATE Documents SET icon=? WHERE id=?");

        auto s_af = s.af();
        s.bind(1, img);
        while(s.step() == SQResult::Row){
            Image   i   = cdb::image(Fragment(s.v_uint64(1)));
            if(!i)
                continue;
        
            auto u_af = u.af();
            u.bind(1, i);
            u.bind(2, doc);
            u.exec();
            return true;
        }
        return false;
    }
    
    void s4_doc_images()
    {
        static thread_local CacheQuery s1("SELECT id,suffix,skb FROM Documents WHERE icon=0");
        static thread_local CacheQuery s2("SELECT id,suffix FROM Documents WHERE skc=?");
        
        
        auto s1_af       = s1.af();
        while(s1.step() == SQResult::Row){
        
            Document    doc(s1.v_uint64(1));
            if(is_image(s1.v_string(2))){
                u_doc_image(doc, doc);
                continue;
            }
        
            std::string             skb(s1.v_string(3));
            auto s2_af     = s2.af();
            s2.bind(1, skb);
            while(s2.step() == SQResult::Row){
                if(!is_image(s2.v_string(2)))
                    continue;
                if(u_doc_image(doc, {s2.v_uint64(1)}))
                    break;
            }
        }
    }
}
