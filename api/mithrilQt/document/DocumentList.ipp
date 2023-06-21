////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DocumentList.hpp"
#include "DocumentModel.hpp"

namespace yq::mithril {
    DocumentList::DocumentList(all_t, QWidget*parent) : DocumentList(ALL, defColumn, parent)
    {
    }
    
    DocumentList::DocumentList(all_t, Column col, QWidget*parent) : 
        IdListT<Document>(new DocumentModel(IdModel::Type::List, ALL), parent)
    {
        model()->setColumn(col);
        model()->reload();
        setWindowTitle("Documents");
    }

    DocumentList::~DocumentList()
    {
    }
    
    DocumentModel*          DocumentList::model()
    {
        return static_cast<DocumentModel*>(m_model);
    }
    
    const DocumentModel*    DocumentList::model() const
    {
        return static_cast<const DocumentModel*>(m_model);
    }
}

