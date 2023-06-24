////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "TagTable.hpp"

namespace yq::mithril {
    std::span<const ColumnSpec>  TagTable::defColumns() 
    {
        static ColumnSpec   s_data[] = { { Column::Key, ColOpt::Icon}, Column::Name };
        return std::span<const ColumnSpec>(std::begin(s_data), std::end(s_data));
    }


    TagTable::TagTable(all_t, QWidget*parent) : TagTable(ALL, defColumns(), parent)
    {
    }
    
    TagTable::TagTable(all_t, std::initializer_list<ColumnSpec> columns, QWidget*parent) : 
        TagTable(ALL, span(columns), parent)
    {
    }
    
    TagTable::TagTable(all_t, std::span<const ColumnSpec> columns, QWidget*parent) : 
        IdTableT<Tag>(ALL, parent)
    {
        model()->addColumns(columns);
        model()->reload();
        setWindowTitle("Tags");
    }

    TagTable::~TagTable()
    {
    }
    
}

