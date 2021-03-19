////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "uAtom.hpp"
#include "uClass.hpp"
#include "yUpdater.hpp"

#include <db/Tag.hpp>
#include <db/Workspace.hpp>
#include <util/SqlQuery.hpp>

UAtom::UAtom(Atom a) : key(cdb::key(a)), id(a.id)
{
}


bool        UAtom::update_classes(const StringSet& newCls)
{
    ClassSet    use;
    for(Class c : cdb::db_classes(newCls)){
        use += c;
        use += uget(c).use.all;
    }
    
    ClassSet    add = use - classes;
    ClassSet    rem = classes - use;
    if(!rem.empty()){
        static thread_local SqlQuery d(wksp::cache(), "DELETE FROM AClasses WHERE atom=? AND class=?");
        auto d_af = d.af();
        d.addBindValue((quint64) id);
        d.addBindValue(cdb::qvar_list(rem));
        d.batch();
    }
    
    if(!add.empty()){
        static thread_local SqlQuery i(wksp::cache(), "INSERT INTO AClasses (atom, class) VALUES (?,?)");
        auto i_af = i.af();
        i.addBindValue((quint64) id);
        i.addBindValue(cdb::qvar_list(add));
        i.batch();
    }

    classes = use;
    return !(add.empty() && rem.empty());
}

bool        UAtom::update_tags(const StringSet& newtag)
{
    TagSet      them    = makeSet(cdb::db_tags(newtag));
    TagSet      add     = them - tags;
    TagSet      rem     = tags - them;
    if(!rem.empty()){
        static thread_local SqlQuery d(wksp::cache(), "DELETE FROM ATags WHERE atom=? AND tag=?");
        auto d_af = d.af();
        d.addBindValue((quint64) id);
        d.addBindValue(cdb::qvar_list(rem));
        d.batch();
    }
    if(!add.empty()){
        static thread_local SqlQuery i(wksp::cache(), "INSERT INTO ATags (atom, tag) VALUES (?,?)");
        auto i_af = i.af();
        i.addBindValue((quint64) id);
        i.addBindValue(cdb::qvar_list(add));
        i.batch();
    }
    tags    = them;
    return !(add.empty() && rem.empty());
}

