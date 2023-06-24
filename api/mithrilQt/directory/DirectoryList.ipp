////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DirectoryList.hpp"

namespace yq::mithril {
    DirectoryList::DirectoryList(all_t, QWidget*parent) : DirectoryList(ALL, defColumn, parent)
    {
    }
    
    DirectoryList::DirectoryList(all_t, Column col, QWidget*parent) : 
        IdListT<Directory>(ALL, parent)
    {
        model()->setColumn(col);
        model()->reload();
        setWindowTitle("Directories");
    }

    DirectoryList::~DirectoryList()
    {
    }
}

