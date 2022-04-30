////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/meta/TypeInfo.hpp>

namespace {


    void    s3_field(Document doc)
    {
        Field               x    = db_field(doc);
        Field::SharedData   data = cdb::update_info(x, DONT_LOCK);
        if(!data)
            return ;
        update_icon(x);

        static thread_local SQ i1("INSERT INTO FAlias (field, alias) VALUES (?, ?)");
        static thread_local SQ i2("INSERT INTO FDataTypes (field, type) VALUES (?, ?)");
        static thread_local SQ i3("INSERT INTO FAtomTypes (field, class) VALUES (?, ?)");
        static thread_local SQ i4("INSERT INTO FTags (field, tag) VALUES (?, ?)");
        static thread_local SQ i5("INSERT INTO FDefClass (field, class) VALUES (?, ?)");
        
        for(const std::string& s : data -> aliases){
            i1.bind(1, x.id);
            i1.bind(2, s);
            i1.exec();
        }
        
        for(const TypeInfo* ti : TypeInfo::find_all(data -> types, true)){
            i2.bind(1, x.id);
            i2.bind(2, (uint64_t) ti->id());
            i2.exec();
        }
        
        for(Class cls : classes(data->atoms, true)){
            i3.bind(1, x.id);
            i3.bind(2, cls.id);
            i3.exec();
        }
        
        for(Tag t : tags(data -> tags, true)){
            i4.bind(1, x.id);
            i4.bind(2, t.id);
            i4.exec();
        }
        
        Class c = class_(x);
        if(c){
            i5.bind(1, x.id);
            i5.bind(2, c.id);
            i5.exec();
        }
        
        for(Class cls : classes(data->classes, true)){
            i5.bind(1, x.id);
            i5.bind(2, cls.id);
            i5.exec();
        }
    }
    
    YQ_INVOKE( u_stage3<s3_field>(fields_folder(), "*.field"); )
}
