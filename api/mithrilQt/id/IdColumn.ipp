////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "IdColumn.hpp"
#include <map>

namespace yq::mithril {

    struct IdColumn::Key {
        IdTypeId    type;
        Column      column;
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
    
    bool             IdColumn::hasColumn(IdTypeId t, Column c)
    {
        Repo& _r = repo();
        return _r.columns.contains({t,c});
    }
    
    std::optional<IdColumn>    IdColumn::create(IdTypeId t, Column c, ColOpts opts)
    {
        Repo& _r = repo();
        auto i = _r.columns.find({t,c});
        if(i != _r.columns.end())
            return (i->second)(opts);
        return {};
    }
    
    void             IdColumn::declare(IdTypeId t, Column c, CreateFN fn)
    {
        Repo& _r = repo();
        _r.columns[{t,c}]   = std::move(fn);
    }

    ////////////////////////////////////////////////////////////////////////////////

    IdColumn::IdColumn() = default;
    IdColumn::~IdColumn() = default;
    
}
