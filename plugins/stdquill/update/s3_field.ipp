////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/meta/TypeInfo.hpp>

namespace {

    std::pair<Field, Field::SharedData>   update_field_info(Document doc)
    {
        Field  x       = db_field(doc);
        Field::SharedData data = merged(x, IS_UPDATE | DONT_LOCK);
        if(!data){
            yWarning() << "Unable to update field '" << key(x) << "' due to lack of data";
            return {};
        }
        
        Category cat = category(data->category);

        static thread_local SQ u1("UPDATE Fields SET name=?,brief=?,multi=?,restrict=?,category=?,pkey=?,expected=?,maxcnt=?,plural=? WHERE id=?");
        u1.bind(1, data->name);
        u1.bind(2, data->brief);
        u1.bind(3, data->multiplicity.value());
        u1.bind(4, data->restriction.value());
        u1.bind(5, cat.id);
        u1.bind(6, data->pkey);
        u1.bind(7, data->expected);
        u1.bind(8, data->max_count);
        u1.bind(9, data->plural);
        u1.bind(10, x.id);
        u1.exec();
        
        return { x, data };
    }
    

    void    s3_field(Document doc)
    {
        Image               img     = best_image(doc);
        Field               x;
        Field::SharedData   data;
        std::tie(x,data) = update_field_info(doc);
        if(!x)
            return ;

        static thread_local SQ u1("UPDATE Fields SET icon=? WHERE id=?");
        static thread_local SQ i1("INSERT INTO FAlias (field, alias) VALUES (?, ?)");
        static thread_local SQ i2("INSERT INTO FDataTypes (field, type) VALUES (?, ?)");
        static thread_local SQ i3("INSERT INTO FAtomTypes (field, class) VALUES (?, ?)");
        static thread_local SQ i4("INSERT INTO FTags (field, tag) VALUES (?, ?)");
        static thread_local SQ i5("INSERT INTO FDefClass (field, class) VALUES (?, ?)");
        
        u1.bind(1, img.id);
        u1.bind(2, x.id);
        u1.exec();
        
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
    
    YQ_INVOKE( u_stage3<s3_field>(fields_folder(), "*.fld"); )
}
