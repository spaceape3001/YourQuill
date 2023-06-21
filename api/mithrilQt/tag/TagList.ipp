////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "TagList.hpp"
#include "TagModel.hpp"

namespace yq::mithril {
    TagList::TagList(all_t, QWidget*parent) : TagList(ALL, defColumn, parent)
    {
    }
    
    TagList::TagList(all_t, Column col, QWidget*parent) : 
        IdListT<Tag>(new TagModel(IdModel::Type::List, ALL), parent)
    {
        model()->setColumn(col);
        model()->reload();
        setWindowTitle("Tags");
    }

    TagList::~TagList()
    {
    }
    
    TagModel*          TagList::model()
    {
        return static_cast<TagModel*>(m_model);
    }
    
    const TagModel*    TagList::model() const
    {
        return static_cast<const TagModel*>(m_model);
    }
}

