////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    void    s3_class(Document doc)
    {
        Class c = db_class(doc);

        Class::SharedData       data    = update_info(c, DONT_LOCK|IS_UPDATE);
        update_icon(c);
        
        static thread_local SQ ia("INSERT INTO CAlias (class, alias) VALUES (?,?)");
        static thread_local SQ ip("INSERT INTO CPrefix (class, prefix) VALUES (?,?)");
        static thread_local SQ is("INSERT INTO CSuffix (class, suffix) VALUES (?,?)");
        static thread_local SQ it("INSERT INTO CTags (class, tag) VALUES (?,?)");
        
        for(const std::string& a : data->aliases){
            ia.bind(1, c.id);
            ia.bind(2, a);
            ia.exec();
        }

        for(const std::string& p : data->prefixes){
            ip.bind(1, c.id);
            ip.bind(2, p);
            ip.exec();
        }
        
        for(const std::string& s : data->suffixes){
            is.bind(1, c.id);
            is.bind(2, s);
            is.exec();
        }
        
        for(const std::string& s : data->tags){
            Tag t = tag(s);
            if(!t){
                yWarning() << "Undefined tag " << s << " in class " << name(c);
                continue;
            }
        
            it.bind(1, c.id);
            it.bind(2, t.id);
            it.exec();
        }
    }

    void    s3_class_bind(Document doc)
    {
        
    
    }

    
    YQ_INVOKE( 
        u_stage3<s3_class>(cdb::classes_folder(), "*.class"); 
        u_stage3<s3_class_bind>(cdb::classes_folder(), "*.class"); 
    )
    
}
