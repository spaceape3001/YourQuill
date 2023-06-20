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
    std::span<const Column>  FragmentModel::defColumns() 
    {
        static Column   s_data[] = { Column::Key, Column::Path };
        return std::span<const Column>(std::begin(s_data), std::end(s_data));
    }

    FragmentModel::FragmentModel(Type t, all_t, std::span<const Column> cols, QObject* parent) : 
        FragmentModel(t, Fragment(), provider::all_fragments(), cols, parent)
    {
    }

    FragmentModel::FragmentModel(Type t, Fragment rt, IdProvider&& prov, std::span<const Column> cols, QObject*parent) : 
        IdModelT<Fragment>(t, rt, std::move(prov), parent)
    {
        for(Column c : cols)
            addColumn(c);
        reload();
    }
    
    FragmentModel::~FragmentModel()
    {
    }
    
    void    FragmentModel::addColumn(Column col)
    {
        switch(col){
        case Column::Id:
            addColumn(column::fragment_id());
            break;
        case Column::Key:
            addColumn(column::fragment_key());
            break;
        case Column::Name:
            addColumn(column::fragment_name());
            break;
        case Column::Path:
            addColumn(column::fragment_path());
            break;
        default:
            break;
        }
    }
    
}
