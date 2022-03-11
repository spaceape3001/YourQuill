////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

ULeaf&            uget(Leaf l)
{
    static Hash<uint64_t, ULeaf*>  data;   // yes, hash, because leaf IDs will not be continuous
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
        
        //data        = {};   // clear it (temporary)
    }
}

#if 0
    // LEFT IN FOR DEBUGGING USE
void    print(const Vector<cdb::KVDiff>& items, size_t depth=1)
{
    for(const cdb::KVDiff& d : items){
        yInfo() << String(depth*2, ' ') << d.key << " (" << d.value << ") " << d.nidx << " {attr " << d.attr.id << "}" 
            << (d.added() ? " INSERT" : "") 
            << (d.modified() ? " MODIFY" : "")
            << (d.deleted() ? " DELETE" : "")
            << (d.indexed() ? " INDEX" : "")
            << (d.subchanged() ? " SUBS": "")
        ;
        
        print(d.sub, depth+1);
    }
}

void    print_tree(const cdb::KVReport& rep)
{
    print(rep.items);
}
#endif

void                ULeaf::do_read()
{
    data            = cdb::merged(leaf, cdb::IsUpdate|cdb::IgnoreContext);
    if(data){
        Atom   a        = cdb::db_atom(doc);
        
        static thread_local SqlQuery u1(wksp::cache(), "UPDATE Leafs SET title=? WHERE id=?");
        auto u1_af = u1.af();
        u1.bind(0, data->title().qString());
        u1.bind(1, id);
        u1.exec();
        
        auto kv     = attr_changes(doc, *data);

        uclasses(a, doc, data->classes());
        utags(a, doc, data->tags());
        uproperties(a, doc, kv);
        
        // Recurssion (TODO)
        execute_removals(kv);
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
    Image i         = uicon(atom, icon);

    static thread_local SqlQuery u(wksp::cache(), "UPDATE Leafs SET icon=? WHERE id=?");
    auto u_af = u.af();
    u.bind(0, i.id);
    u.bind(1, id);
    u.exec();
}

