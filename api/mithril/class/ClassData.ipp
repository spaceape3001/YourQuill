////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ClassData.hpp"
#include <yq/container/set_utils.hpp>
//#include <0/basic/CollectionUtils.hpp>
#include <0/basic/TextUtils.hpp>

namespace yq::mithril {

    Class::Data&      Class::Data::merge(const Class::Data&b, bool fOverride)
    {
        set_if_empty(name, b.name, fOverride);
        set_if_empty(plural, b.plural, fOverride);
        set_if_empty(brief, b.brief, fOverride);
        set_if_empty(notes, b.notes, fOverride);
        set_if_empty(folder, b.folder, fOverride);
        set_if_empty(category, b.category, fOverride);
        use += b.use;
        reverse += b.reverse;
        sources += b.sources;
        targets += b.targets;
        set_if_empty(binding, b.binding, fOverride);
        suffixes += b.suffixes;
        prefixes += b.prefixes;
        aliases += b.aliases;
        tags += b.tags;
        set_if_empty(url, b.url, fOverride);
        set_if_empty(dev_url, b.dev_url, fOverride);
        return *this;
    }
}
