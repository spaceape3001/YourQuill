////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "LeafFile.hpp"
#include <yq/text/text_utils.hpp>

namespace yq {

    std::string_view      LeafFile::abbr() const
    {
        return value(kv::key(zAbbr));
    }

    void        LeafFile::abbr(const std::string_view&s)
    {
        set(zAbbr, s, true);
    }

    std::string_view      LeafFile::brief() const
    {
        return value(zBrief);
    }

    void        LeafFile::brief(const std::string_view&s)
    {
        set(zBrief, s, true);
    }

    string_view_set_t LeafFile::classes() const
    {
        return values_set(zClass);
    }

    void        LeafFile::classes(const string_view_set_t&s)
    {
        set(zClass, join(s, ','));
    }

    void        LeafFile::classes(const string_set_t&s)
    {
        set(zClass, join(s, ','));
    }

    Context     LeafFile::context() const
    {
        return Context();
//        return Context(format(), body);
    }


    Format      LeafFile::format() const
    {
        const KeyValue* a = first(zFormat);
        if(!a)
            return defFormat;
        auto r = Format::value_for(a->data);
        if(r.good)
            return r.value;
        return defFormat;
    }

    void        LeafFile::format(Format f)
    {
        set(zFormat, f.key(), true);
    }

    std::string_view      LeafFile::notes() const
    {
        return value(zNote);
    }

    void        LeafFile::notes(const std::string_view&s)
    {
        set(zNote, s, true);
    }

    string_view_set_t   LeafFile::tags() const
    {
        return values_set(zTag);
    }

    void        LeafFile::tags(const string_set_t&s)
    {
        set(zTag, join(s, ','));
    }

    void        LeafFile::tags(const string_view_set_t&s)
    {
        set(zTag, join(s, ','));
    }

    std::string_view      LeafFile::title() const
    {
        return value(zTitle);
    }

    void        LeafFile::title(const std::string_view&s)
    {
        set(zTitle, s, true);
    }
}
