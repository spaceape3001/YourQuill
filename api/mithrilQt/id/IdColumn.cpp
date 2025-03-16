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
        std::map<Key, CreateFN>                     columns;
        std::map<IdTypeId, ColumnSpec>              defList;
        std::map<IdTypeId, std::vector<ColumnSpec>> defTable;
        
        Repo() 
        {
        }
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

    ColumnSpec               IdColumn::defaultList(IdTypeId t)
    {
        Repo& _r = repo();
        auto i = _r.defList.find(t);
        if(i != _r.defList.end())
            return i->second;
        return ColumnSpec{};
    }
    
    std::span<const ColumnSpec>  IdColumn::defaultTable(IdTypeId t)
    {
        Repo& _r = repo();
        auto i = _r.defTable.find(t);
        if(i != _r.defTable.end())
            return i->second;
        return {};
    }
    

    void    IdColumn::set_defaultList(IdTypeId t, ColumnSpec cspec)
    {
        Repo& _r = repo();
        _r.defList[t]   = cspec;
    }
    
    void    IdColumn::set_defaultTable(IdTypeId t, std::initializer_list<ColumnSpec> cols)
    {
        Repo& _r = repo();
        _r.defTable[t]  = std::vector<ColumnSpec>(cols.begin(), cols.end());
    }
    

    ////////////////////////////////////////////////////////////////////////////////

    IdColumn::IdColumn() = default;
    IdColumn::~IdColumn() = default;
    
}
