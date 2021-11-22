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


namespace {
    struct UAtom {
        const Atom          atom;
        const Document      doc;
        String              sk;
        ClassSet            classes;
        
        UAtom(Atom a, Document d) : atom(a), doc(d), sk(cdb::skey(a)), classes(makeSet(cdb::classes(a)))
        {
        }

        Property    add_property(const cdb::KVDiff& kv, AttrKind kind, const String& type=String(), uint64_t rid=0ULL)
        {
            static thread_local cdb::SQ i("INSERT INTO Properties (atom, uid, k, value, kind, type, rid) VALUES (?, ?, ?, ?, ?, ?, ?)");
            auto i_af = i.af();
            i.bind(0, atom.id);
            i.bind(1, kv.uid);
            i.bind(2, kv.key);
            i.bind(3, kv.value.trimmed());
            i.bind(4, (int) kind.value());
            i.bind(5, type);
            i.bind(6, rid);
            i.exec();
            
            Property    prop{i.lastInsertIdU64()};
            static thread_local cdb::SQ i2("INSERT OR REPLACE INTO PAttribute (prop, attr, implied) VALUES (?, ?, 0)");
            auto i2_af = i.af();
            i2.bind(0, prop.id);
            i2.bind(1, kv.attr.id);
            i2.exec();
            return prop;
        }


        Property     add_value(const cdb::KVDiff& kv)
        {
            return  add_property(kv, AttrKind::Value);
        }
        
        std::pair<Property,Atom>    add_atom(const cdb::KVDiff& kv)
        {
            //static const uint64_t   cnt = 0;
            String     k   = sk + '#' + kv.key + '.' + kv.value;
            if(!kv.uid.empty())
                k += '.' + kv.uid;
            Atom        a   = cdb::db_atom(doc, k);
            Property    p   = add_property(kv, AttrKind::Atom, String(), a.id);
            return {p,a};
        }
    };

    //struct PropInfo {
        //bool        hasInit = false;
        //PropInfo&   init()
        //{
            //if(!hasInit){
                //hasInit = true;
            //}
            //return *this;
        //}
    //};


    //struct PropInfoDB {
        //Map<QString,PropInfo>   props;
        //Atom                    atom;
        //Document                doc;
        
        //PropInfoDB(Atom a, Document d, const ClassSet& cs) : atom(a), doc(d), classes(cs) {}
        
        //PropInfo&   info(const QString& k) 
        //{   
            //return props[k].init();
        //}
        
        //Property    addProperty(const 
        
    //};
}

Vector<Property>           uproperties(Atom atom, Document doc, const Vector<cdb::KVDiff>& items)
{
    UAtom                   u(atom, doc);
    Vector<Property>        ret;
    
//    Map<QString,PropInfo>   infos;  //  this is a cache.... as needed.....
    for(auto & kv : items){
        Atom            a;
        if(kv.added()){
            if(kv.sub.empty()){ 
                ret << u.add_value(kv);
            } else {
                auto p  = u.add_atom(kv);
                ret << p.first;
                ret += uproperties(p.second, doc, kv.sub);
            }
        } else if(kv.modified()){
            
        } else if(kv.deleted()){
            
        } 
        if(kv.subchanged()){
        }
    }
    return ret;
}

Vector<Property>           uproperties(Atom atom, Document doc, const cdb::KVReport& attributes)
{
    return uproperties(atom, doc, attributes.items);
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





    
