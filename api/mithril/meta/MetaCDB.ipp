////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/meta/Meta.hpp>
#include <0/meta/Meta.hpp>

namespace yq::mithril::cdb {
    std::vector<Meta>   all_metas(Sorted sorted)
    {
        std::vector<Meta>           ret;

        std::vector<const MetaBase*>    all = MetaBase::all();
        if(sorted){
            std::stable_sort(all.begin(), all.end(), [](const MetaBase* a, const MetaBase* b) -> bool {
                return a->name() < b->name();
            });
        }
        
        ret.reserve(all.size());
        for(const MetaBase* mb : all)
            ret.push_back(Meta(mb->id()));
        return ret;
    }

    std::string         key(Meta m)
    {
        if(m.id >= 1ULL << 32)  // out of range
            return std::string();
        const MetaBase* mb  = MetaBase::lookup(m.id);
        if(!mb)
            return std::string();
        return std::string(mb -> name());
    }

    std::string         name(Meta m)
    {
        return key(m);
    }
}
