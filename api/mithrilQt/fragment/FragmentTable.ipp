////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FragmentTable.hpp"
#include "FragmentModel.hpp"


namespace yq::mithril {

    FragmentTable::FragmentTable(all_t, QWidget*parent) : FragmentTable(ALL, FragmentModel::defColumns(), parent)
    {
    }
    
    FragmentTable::FragmentTable(all_t, std::initializer_list<Column> columns, QWidget*parent) : 
        FragmentTable(ALL, span(columns), parent)
    {
    }
    
    FragmentTable::FragmentTable(all_t, std::span<const Column> columns, QWidget*parent) : 
        IdTableT<Fragment>(new FragmentModel(IdModel::Type::Table, ALL, columns), parent)
    {
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

