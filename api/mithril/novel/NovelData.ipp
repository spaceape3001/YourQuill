////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#error "Do NOT include (for now)"

#pragma once

#include "NovelData.hpp"

namespace yq::mithril {

    Novel::Data&   Novel::Data::merge(const Data&b, bool)
    {
        //attrs.fusion(b.attrs);
        //context += b.context;
        return *this;
    }

    void            Novel::Data::reset()
    {
        //attrs       = KVTree();
        //context.clear();
    }

#if 0

    std::string_view      Novel::Data::abbr() const
    {
        return attrs.value(kv::key(zAbbr));
    }

    void        Novel::Data::abbr(const std::string_view&s)
    {
        attrs.set(zAbbr, s, true);
    }

    std::string_view      Novel::Data::brief() const
    {
        return attrs.value(zBrief);
    }

    void        Novel::Data::brief(const std::string_view&s)
    {
        attrs.set(zBrief, s, true);
    }

    string_view_set_t Novel::Data::classes() const
    {
        string_view_set_t   tmp;
        for(std::string_view v : attrs.values_set(zClass)){
            tmp.insert(trimmed(v));
        }
        return tmp;
    }

    void        Novel::Data::classes(const string_view_set_t&s)
    {
        attrs.set(zClass, join(s, ','));
    }

    void        Novel::Data::classes(const string_set_t&s)
    {
        attrs.set(zClass, join(s, ','));
    }


    std::string_view      Novel::Data::notes() const
    {
        return attrs.value(zNote);
    }

    void        Novel::Data::notes(const std::string_view&s)
    {
        attrs.set(zNote, s, true);
    }

    string_view_set_t   Novel::Data::tags() const
    {
        return attrs.values_set(zTag);
    }

    void        Novel::Data::tags(const string_set_t&s)
    {
        attrs.set(zTag, join(s, ','));
    }

    void        Novel::Data::tags(const string_view_set_t&s)
    {
        attrs.set(zTag, join(s, ','));
    }

    std::string_view      Novel::Data::title() const
    {
        return attrs.value(zTitle);
    }

    void        Novel::Data::title(const std::string_view&s)
    {
        attrs.set(zTitle, s, true);
    }
#endif
    
}
