////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "uAtom.hpp"
#include "uLeaf.hpp"
#include "yUpdater.hpp"

#include <db/Image.hpp>
#include <db/Workspace.hpp>
#include <srv/Importer.hpp>
#include <util/Hash.hpp>
#include <util/SqlQuery.hpp>

ULeaf::ULeaf(Leaf l) : key(cdb::key(l)), id(l.id), 
    folder(cdb::detail_folder(l)), 
    atom(cdb::db_atom(Document{l.id}))
{
    static thread_local SqlQuery u(wksp::cache(), "UPDATE Leafs SET atom=? WHERE id=?");
    auto u_af = u.af();
    u.bind(0, atom.id);
    u.bind(1, id);
    u.exec();
}

Image               ULeaf::explicit_icon() const
{
    for(const char* x : Image::kSupportedExtensions){
        Document    d   = cdb::document(key + "." + x);
        if(d && cdb::fragments_count(d))
            return cdb::image(d);
    }
    return Image{};
}

ULeaf&            uget(Leaf l)
{
    static Hash<uint64_t, ULeaf*>  data;   // yes, hash, because class IDs will not be continuous
    ULeaf* p    = data.get(l.id,nullptr);
    if(!p){
        p       = new ULeaf(l);
        data[l.id]  = p;
    }
    return *p;
}


namespace {
        
    void    on_read(ULeaf& u)
    {
        u.data      = cdb::merged(u.leaf, cdb::IsUpdate|cdb::IgnoreContext);
        UAtom a     = uget(cdb::db_atom(u.doc));
        
        
        static thread_local SqlQuery u1(wksp::cache(), "UPDATE Leafs SET title=? WHERE id=?");
        auto u1_af = u1.af();
        u1.bind(0, u.data->title().qString());
        u1.bind(1, u.id);
        u1.exec();
        
        a.update_classes(u.data->classes());
        a.update_tags(u.data->tags());
        
        
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
        on_read(uget(l));
        on_exterior_read(l);
    }
    

}

void    init_leaf()
{
    for(Document d : cdb::all_documents_suffix("y")){    // create leafs
        cdb::db_atom(d);
        cdb::db_leaf(d);
    }
    for(Leaf l : cdb::all_leafs())
        on_read(uget(l));
    for(Leaf l : cdb::all_leafs())
        on_exterior_read(l);
    on_change("*.y",   on_leaf_change);
}
