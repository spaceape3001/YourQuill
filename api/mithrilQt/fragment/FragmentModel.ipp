////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FragmentModel.hpp"
#include "FragmentColumn.hpp"
#include <mithril/fragment/FragmentProvider.hpp>

namespace yq::mithril {
    std::optional<IdColumn>     FragmentModel::resolve(Column col, ColOpts opts)
    {
        switch(col){
        case Column::Id:
            return column::fragment_id(opts);
        case Column::Key:
            return column::fragment_key(opts);
        case Column::Name:
            return column::fragment_name(opts);
        case Column::Path:
            return column::fragment_path(opts);
            break;
        default:
            return {};
        }
    }

    FragmentModel::FragmentModel(Type t, all_t, QObject* parent) : 
        FragmentModel(t, Fragment(), provider::all_fragments(), parent)
    {
    }

    FragmentModel::FragmentModel(Type t, Fragment rt, IdProvider&& prov, QObject*parent) : 
        IdModelT<Fragment>(t, rt, std::move(prov), parent)
    {
    }
    
    FragmentModel::~FragmentModel()
    {
    }
    
    void    FragmentModel::addColumn(Column col, ColOpts opts)
    {
        auto    cc  = resolve(col, opts);
        if(cc)
            addColumn(std::move(*cc));
    }

    void    FragmentModel::addColumns(std::span<const Column> columns)
    {
        for(Column c : columns)
            addColumn(c);
    }
    
    void    FragmentModel::setColumn(Column col, ColOpts opts)
    {
        auto    cc = resolve(col, opts);
        if(cc)
            setColumn(std::move(*cc));
    }
}
