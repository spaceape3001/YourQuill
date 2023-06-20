////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "TagTable.hpp"
#include "TagModel.hpp"


namespace yq::mithril {
    std::span<const Column>  TagTable::defColumns() 
    {
        static Column   s_data[] = { Column::Key, Column::Name };
        return std::span<const Column>(std::begin(s_data), std::end(s_data));
    }


    TagTable::TagTable(all_t, QWidget*parent) : TagTable(ALL, defColumns(), parent)
    {
    }
    
    TagTable::TagTable(all_t, std::initializer_list<Column> columns, QWidget*parent) : 
        TagTable(ALL, span(columns), parent)
    {
    }
    
    TagTable::TagTable(all_t, std::span<const Column> columns, QWidget*parent) : 
        IdTableT<Tag>(new TagModel(IdModel::Type::Table, ALL), parent)
    {
        model()->addColumns(columns);
        model()->reload();
        setWindowTitle("Tags");
    }

    TagTable::~TagTable()
    {
    }
    
    
    TagModel*          TagTable::model()
    {
        return static_cast<TagModel*>(m_model);
    }
    
    const TagModel*    TagTable::model() const
    {
        return static_cast<const TagModel*>(m_model);
    }
}

