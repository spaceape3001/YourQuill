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

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void         ULeaf::init_read()
{
   for(Document d : cdb::all_documents_suffix("y")){    // create leafs
        cdb::db_atom(d);
        cdb::db_leaf(d);
    }
    for(Leaf l : cdb::all_leafs()){
        ULeaf& u = uget(l);
        u.do_read();
        u.do_icon();
    }
 }

void         ULeaf::init_link()
{
    for(Leaf l : cdb::all_leafs())
        uget(l).do_link();
    on_change("*.y", &ULeaf::on_leaf_change);
    for(const char* x : Image::kSupportedExtensions){
        on_add("*." + QString(x), &ULeaf::on_image_change);
        on_remove("*." + QString(x), &ULeaf::on_image_change);
    }
}

void         ULeaf::on_image_change(Fragment f)
{
    if(cdb::folder(f) == cdb::classes_folder()){
        for(Leaf l : cdb::all_leafs()){
            ULeaf& u = uget(l);
            if(!u.icon)
                u.push_icon();
        }
    } else {
        Image   i{f.id};
        Leaf    l   = cdb::leaf(cdb::base_key(f));
        if(!l)
            return ;
        ULeaf&  u   = uget(l);
        if(u.icon != i)
            u.do_icon();
    }
}

void         ULeaf::on_leaf_change(Fragment f)
{
    ULeaf&  l   = uget(cdb::db_leaf(cdb::document(f)));
    l.do_read();
    l.do_link();
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

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

void                ULeaf::do_icon()
{
    icon            = explicit_icon();
    push_icon();
}

void                ULeaf::do_link()
{
    if(data){
        //  TODO.... (massive)
    }
}

void                ULeaf::do_read()
{
    data            = cdb::merged(leaf, cdb::IsUpdate|cdb::IgnoreContext);
    if(data){
        UAtom& a        = uget(cdb::db_atom(doc));
        
        static thread_local SqlQuery u1(wksp::cache(), "UPDATE Leafs SET title=? WHERE id=?");
        auto u1_af = u1.af();
        u1.bind(0, data->title().qString());
        u1.bind(1, id);
        u1.exec();
        
        a.update_classes(data->classes());
        a.update_tags(data->tags());
        
        // Recurssion (TODO)
    }
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

void                ULeaf::push_icon()
{
    Image i         = uget(atom).update_icon(icon);

    static thread_local SqlQuery u(wksp::cache(), "UPDATE Leafs SET icon=? WHERE id=?");
    auto u_af = u.af();
    u.bind(0, i.id);
    u.bind(1, id);
    u.exec();
}

