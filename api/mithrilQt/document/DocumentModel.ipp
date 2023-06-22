////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DocumentModel.hpp"
#include "DocumentColumn.hpp"
#include <mithril/document/DocumentProvider.hpp>

namespace yq::mithril {
    DocumentModel::DocumentModel(Type t, all_t, QObject* parent) : 
        DocumentModel(t, Document(), provider::all_documents(), parent)
    {
    }

    DocumentModel::DocumentModel(Type t, Document rt, IdProvider&& prov, QObject*parent) : 
        IdModelT<Document>(t, rt, std::move(prov), parent)
    {
    }
    
    DocumentModel::~DocumentModel()
    {
    }
}
