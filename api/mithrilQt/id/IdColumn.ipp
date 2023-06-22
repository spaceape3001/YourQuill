////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "IdColumn.hpp"
#include <map>

namespace yq::mithril {

    struct IdColumn::Key {
        IdType  type;
        Column  column;
        constexpr auto operator<=>(const Key&) const noexcept = default;
    };

    struct IdColumn::Repo {
        std::map<Key, CreateFN>     columns;
    };
    
    IdColumn::Repo& IdColumn::repo()
    {
        static Repo s_repo;
        return s_repo;
    }
    
    #define REPO    \
        Repo&   _r  = repo();

    bool             IdColumn::hasColumn(IdType t, Column c)
    {
        REPO
        return _r.columns.contains({t,c});
    }
    
    std::optional<IdColumn>    IdColumn::create(IdType t, Column c, ColOpts opts)
    {
        REPO
        auto i = _r.columns.find({t,c});
        if(i != _r.columns.end())
            return (i->second)(opts);
        return {};
    }
    
    void             IdColumn::declare(IdType t, Column c, CreateFN fn)
    {
        REPO
        _r.columns[{t,c}]   = std::move(fn);
    }

    ////////////////////////////////////////////////////////////////////////////////

    IdColumn::IdColumn() = default;
    IdColumn::~IdColumn() = default;
    
}
