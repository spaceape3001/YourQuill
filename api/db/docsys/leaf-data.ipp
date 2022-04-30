////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace yq {
    Leaf::Data&   Leaf::Data::merge(const Data&b, bool fOverride)
    {
        attrs.fusion(b.attrs);
        context += b.context;
        return *this;
    }

    void            Leaf::Data::reset()
    {
        attrs       = KVTree();
        context.clear();
    }

    std::string_view      Leaf::Data::abbr() const
    {
        return attrs.value(kv::key(zAbbr));
    }

    void        Leaf::Data::abbr(const std::string_view&s)
    {
        attrs.set(zAbbr, s, true);
    }

    std::string_view      Leaf::Data::brief() const
    {
        return attrs.value(zBrief);
    }

    void        Leaf::Data::brief(const std::string_view&s)
    {
        attrs.set(zBrief, s, true);
    }

    string_view_set_t Leaf::Data::classes() const
    {
        return attrs.values_set(zClass);
    }

    void        Leaf::Data::classes(const string_view_set_t&s)
    {
        attrs.set(zClass, join(s, ','));
    }

    void        Leaf::Data::classes(const string_set_t&s)
    {
        attrs.set(zClass, join(s, ','));
    }


    std::string_view      Leaf::Data::notes() const
    {
        return attrs.value(zNote);
    }

    void        Leaf::Data::notes(const std::string_view&s)
    {
        attrs.set(zNote, s, true);
    }

    string_view_set_t   Leaf::Data::tags() const
    {
        return attrs.values_set(zTag);
    }

    void        Leaf::Data::tags(const string_set_t&s)
    {
        attrs.set(zTag, join(s, ','));
    }

    void        Leaf::Data::tags(const string_view_set_t&s)
    {
        attrs.set(zTag, join(s, ','));
    }

    std::string_view      Leaf::Data::title() const
    {
        return attrs.value(zTitle);
    }

    void        Leaf::Data::title(const std::string_view&s)
    {
        attrs.set(zTitle, s, true);
    }
}
