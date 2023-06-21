////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DirectoryList.hpp"
#include "DirectoryModel.hpp"

namespace yq::mithril {
    DirectoryList::DirectoryList(all_t, QWidget*parent) : DirectoryList(ALL, defColumn, parent)
    {
    }
    
    DirectoryList::DirectoryList(all_t, Column col, QWidget*parent) : 
        IdListT<Directory>(new DirectoryModel(IdModel::Type::List, ALL), parent)
    {
        model()->setColumn(col);
        model()->reload();
        setWindowTitle("Directories");
    }

    DirectoryList::~DirectoryList()
    {
    }
    
    DirectoryModel*          DirectoryList::model()
    {
        return static_cast<DirectoryModel*>(m_model);
    }
    
    const DirectoryModel*    DirectoryList::model() const
    {
        return static_cast<const DirectoryModel*>(m_model);
    }
}

