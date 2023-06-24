////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DocumentList.hpp"

namespace yq::mithril {
    DocumentList::DocumentList(all_t, QWidget*parent) : DocumentList(ALL, defColumn, parent)
    {
    }
    
    DocumentList::DocumentList(all_t, Column col, QWidget*parent) : 
        IdListT<Document>(ALL, parent)
    {
        model()->setColumn(col);
        model()->reload();
        setWindowTitle("Documents");
    }

    DocumentList::~DocumentList()
    {
    }
}

