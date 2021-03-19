////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "uTag.hpp"
#include <db/Image.hpp>
#include <db/Leaf.hpp>
#include <db/Workspace.hpp>
#include <srv/Importer.hpp>
#include <util/Logging.hpp>
#include <util/SqlQuery.hpp>

UTag&      uget(Tag a)
{
    static Vector<UTag*>       data(1024, nullptr);
    data.resize_if_under(a.id+1,256,nullptr);
    UTag*& p = data[a.id];
    if(!p)
        p   = new UTag(a);
    return *p;
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void  UTag::init_read()
{
    for(Document d : cdb::documents_by_suffix(cdb::tags_folder(), "tag")){
        UTag&   u   = uget(cdb::db_tag(d));
        u.do_read();
        u.do_icon();
    }
}

void  UTag::init_link()
{
    for(Tag  t : cdb::all_tags())
        uget(t).do_link();
    on_add("*.tag", &UTag::on_tag_added);
    on_modify("*.tag", &UTag::on_tag_change);
    on_remove("*.tag", &UTag::on_tag_change);
    for(const char* x : Image::kSupportedExtensions){
        on_add("*." + QString(x), &UTag::on_image_change);
        on_remove("*." + QString(x), &UTag::on_image_change);
    }
}

void  UTag::on_image_change(Fragment frag)
{
    if(cdb::folder(frag) != cdb::tags_folder())
        return ;
    uget(cdb::db_tag(cdb::skeyb(frag))).do_icon();
}

void  UTag::on_tag_added(Fragment frag)
{
    if(cdb::folder(frag) != cdb::tags_folder())
        return ;

    auto& u     = uget(cdb::db_tag(frag));
    u.do_read();
    u.do_link();
    u.do_icon();
}

void  UTag::on_tag_change(Fragment frag)
{
    if(cdb::folder(frag) != cdb::tags_folder())
        return ;
        
    auto& u     = uget(cdb::db_tag(frag));
    u.do_read();
    u.do_link();
}


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

UTag::UTag(Tag t) : key(cdb::key(t)), id(t.id)
{
}

void    UTag::do_icon()
{
    Image       i   = explicit_icon();
    if(i == icon)
        return ;
    icon        = i;
    static thread_local SqlQuery u(wksp::cache(), "UPDATE Tags SET icon=? WHERE id=?");
    auto u_af = u.af();
    u.bind(0, icon.id);
    u.bind(1, id);
    u.exec();
}

void    UTag::do_link()
{
    if(data){
        Leaf l  = cdb::leaf(data->leaf.qString());
        static thread_local SqlQuery    u(wksp::cache(), "UPDATE Tags SET leaf=? WHERE id=?");
        auto u_af = u.af();
        u.bind(0, l.id);
        u.bind(1, id);
        u.exec();
    }
}

void    UTag::do_read()
{
    data    = cdb::merged(tag, cdb::IsUpdate);
    if(data){
        static thread_local SqlQuery    u(wksp::cache(), "UPDATE Tags SET name=?,brief=? WHERE id=?");
        auto u_af = u.af();
        u.bind(0, data->name.qString());
        u.bind(1, data->brief.qString());
        u.bind(2, id);
        u.exec();
    } 
}

Image   UTag::explicit_icon() const
{
    for(const char* x : Image::kSupportedExtensions){
        Document    d   = cdb::document(cdb::tags_folder(), key + "." + x);
        if(d && cdb::fragments_count(d))
            return cdb::image(d);
    }
    return Image{};
}


