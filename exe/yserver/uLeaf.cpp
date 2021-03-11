////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "yUpdater.hpp"

#include <db/Workspace.hpp>
#include <srv/Importer.hpp>
#include <util/SqlQuery.hpp>


namespace {
        
    void    on_interior_read(Leaf l)
    {
        auto m = cdb::merged(l, cdb::IsUpdate);

        Atom a  = cdb::db_atom(cdb::document(l));
        Set<Class> old = makeSet(cdb::classes(a));
        
        Set<Class> use;
        for(String s : m->classes()){
            Class c = cdb::db_class(s.qString());
            use << c;
            use |= makeSet(cdb::uses(c));
        }
        
        static thread_local SqlQuery u(wksp::cache(), "UPDATE Leafs SET atom=?,title=? WHERE id=?");
        auto u_af = u.af();
        u.bind(0, a.id);
        u.bind(1, m->title().qString());
        u.bind(2, l.id);
        u.exec();
        
        
        static thread_local SqlQuery u2(wksp::cache(), "UPDATE Atoms SET leaf=?,title=? WHERE id=?");
        auto u2_af = u2.af();
        u2.bind(0, l.id);
        u2.bind(1, m->title().qString());
        u2.bind(2, a.id);
        u2.exec();
        
        Set<Class>  added   = use - old;
        if(!added.empty()){
            QVariantList    id, cls;
            for(Class c : added){
                id  << (quint64) a.id;
                cls << (quint64) c.id;
            }
            
            static thread_local SqlQuery i(wksp::cache(), "INSERT INTO AClasses (atom, class) VALUES (?,?)");
            auto i_af = i.af();
            i.addBindValue(id);
            i.addBindValue(cls);
            i.batch();
        }
        
        Set<Class>  removed = old - use;
        if(!removed.empty()){
            QVariantList    id, cls;
            for(Class c : removed){
                id  << (quint64) a.id;
                cls << (quint64) c.id;
            }
            
            static thread_local SqlQuery d(wksp::cache(), "DELETE FROM AClasses WHERE atom=? AND class=?");
            auto d_af = d.af();
            d.addBindValue(id);
            d.addBindValue(cls);
            d.batch();
        }
        
        
        //  MORE (TODO)
    }
    
    void    on_exterior_read(Leaf l)
    {
        auto m = cdb::merged(l, cdb::IsUpdate);
//        auto m = l.leaf.merge(Leaf::OptUpdate | Leaf::OptAttributes);
    }
    

    void    on_leaf_change(Fragment fragment)
    {
        Document    doc = cdb::document(fragment);
        Leaf        l   = cdb::db_leaf(doc);
        on_interior_read(l);
        on_exterior_read(l);
    }
    

}

void    init_leaf()
{
    for(Document d : cdb::all_documents_suffix("y"))    // create leafs
        cdb::db_leaf(d);
    for(Leaf l : cdb::all_leafs())
        on_interior_read(l);
    for(Leaf l : cdb::all_leafs())
        on_exterior_read(l);
    on_change("*.y",   on_leaf_change);
}
