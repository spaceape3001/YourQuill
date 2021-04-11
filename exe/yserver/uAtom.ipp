////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#if 0
namespace {
    struct PropInfo {
        Vector<Field>   field;
        Class           class_;
    };
}
#endif

AddRem<Class>       uclasses(Atom a, Document doc, const StringSet& ncls)
{
    ClassVec        old = cdb::classes(a);
    AddRem<Class>   delta(cdb::classes(a,doc), uresolve(cdb::db_classes(ncls)));
    
    if(!delta.rem.empty()){
        static thread_local SqlQuery d(wksp::cache(), "DELETE FROM AClasses WHERE atom=? AND doc=? AND class=?");
        auto d_af = d.af();
        d.addBindValue((quint64) a.id);
        d.addBindValue((quint64) doc.id);
        d.addBindValue(cdb::qvar_list(delta.rem));
        d.batch();
    }
    
    if(!delta.add.empty()){
        static thread_local SqlQuery i(wksp::cache(), "INSERT INTO AClasses (atom, doc, class) VALUES (?,?,?)");
        auto i_af = i.af();
        i.addBindValue((quint64) a.id);
        i.addBindValue((quint64) doc.id);
        i.addBindValue(cdb::qvar_list(delta.add));
        i.batch();
    }
    return AddRem<Class>(old, cdb::classes(a));
}


Image   uicon(Atom a, Image img)
{
    Image   old = cdb::icon(a);
    if(old == img)
        return img;

    static thread_local SqlQuery u(wksp::cache(), "UPDATE Atoms SET icon=? WHERE id=?");
    auto u_af = u.af();
    u.bind(0, img.id);
    u.bind(1, a.id);
    u.exec();
    
    return img;
}

void            uproperties(Atom atom, Document doc, const cdb::KVReport& attributes)
{
//    Map<QString,PropInfo>   infos;  //  this is a cache.... as needed.....
    for(auto & kv : attributes.items){
        if(kv.added()){
        } else if(kv.modified()){
        } else if(kv.deleted()){
        } 
        if(kv.subchanged()){
        }
    }
}

AddRem<Tag>     utags(Atom a, Document doc, const StringSet& ntags)
{
    if(!a || !doc)
        return {};
    
    TagVec      old = cdb::tags(a);
    AddRem<Tag> delta(cdb::tags(a,doc), cdb::db_tags(ntags));
    if(!delta.rem.empty()){
        static thread_local SqlQuery d(wksp::cache(), "DELETE FROM ATags WHERE atom=? AND tag=?");
        auto d_af = d.af();
        d.addBindValue((quint64) a.id);
        d.addBindValue((quint64) doc.id);
        d.addBindValue(cdb::qvar_list(delta.rem));
        d.batch();
    }
    if(!delta.add.empty()){
        static thread_local SqlQuery i(wksp::cache(), "INSERT INTO ATags (atom, tag) VALUES (?,?)");
        auto i_af = i.af();
        i.addBindValue((quint64) a.id);
        i.addBindValue((quint64) doc.id);
        i.addBindValue(cdb::qvar_list(delta.add));
        i.batch();
    }
    return AddRem<Tag>(old, cdb::tags(a));
}





    
