////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "uTag.hpp"
#include <db/Image.hpp>

UTag::UTag(Tag t) : key(cdb::key(t)), id(t.id)
{
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


UTag&      uget(Tag a)
{
    static Vector<UTag*>       data(1024, nullptr);
    data.resize_if_under(a.id+1,256,nullptr);
    UTag*& p = data[a.id];
    if(!p)
        p   = new UTag(a);
    return *p;
}
