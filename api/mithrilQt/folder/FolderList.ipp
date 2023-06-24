////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FolderList.hpp"

namespace yq::mithril {
    FolderList::FolderList(all_t, QWidget*parent) : FolderList(ALL, defColumn, parent)
    {
    }
    
    FolderList::FolderList(all_t, Column col, QWidget*parent) : 
        IdListT<Folder>(ALL, parent)
    {
        model()->setColumn(col);
        model()->reload();
        setWindowTitle("Folders");
    }

    FolderList::~FolderList()
    {
    }
}

