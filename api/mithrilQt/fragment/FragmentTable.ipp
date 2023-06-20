////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FragmentTable.hpp"
#include "FragmentModel.hpp"


namespace yq::mithril {
    std::span<const Column>  FragmentTable::defColumns() 
    {
        static Column   s_data[] = { Column::Key, Column::Path };
        return std::span<const Column>(std::begin(s_data), std::end(s_data));
    }


    FragmentTable::FragmentTable(all_t, QWidget*parent) : FragmentTable(ALL, defColumns(), parent)
    {
    }
    
    FragmentTable::FragmentTable(all_t, std::initializer_list<Column> columns, QWidget*parent) : 
        FragmentTable(ALL, span(columns), parent)
    {
    }
    
    FragmentTable::FragmentTable(all_t, std::span<const Column> columns, QWidget*parent) : 
        IdTableT<Fragment>(new FragmentModel(IdModel::Type::Table, ALL), parent)
    {
        model()->addColumns(columns);
        model()->reload();
        setWindowTitle("Fragments");
    }

    FragmentTable::~FragmentTable()
    {
    }
    
    
    FragmentModel*          FragmentTable::model()
    {
        return static_cast<FragmentModel*>(m_model);
    }
    
    const FragmentModel*    FragmentTable::model() const
    {
        return static_cast<const FragmentModel*>(m_model);
    }
}

