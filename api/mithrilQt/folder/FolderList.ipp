////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FolderList.hpp"
#include "FolderModel.hpp"

namespace yq::mithril {
    FolderList::FolderList(all_t, QWidget*parent) : FolderList(ALL, defColumn, parent)
    {
    }
    
    FolderList::FolderList(all_t, Column col, QWidget*parent) : 
        IdListT<Folder>(new FolderModel(IdModel::Type::List, ALL), parent)
    {
        model()->setColumn(col);
        model()->reload();
        setWindowTitle("Folders");
    }

    FolderList::~FolderList()
    {
    }
    
    FolderModel*          FolderList::model()
    {
        return static_cast<FolderModel*>(m_model);
    }
    
    const FolderModel*    FolderList::model() const
    {
        return static_cast<const FolderModel*>(m_model);
    }
}

