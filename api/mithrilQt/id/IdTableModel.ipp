////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "IdTableModel.hpp"

namespace yq::mithril {
    IdTableModel::IdTableModel(Id i, IdProvider&& p, QObject*parent) :
        IdModel(Type::Table, i, std::move(p), parent)
    {
    }
        
    IdTableModel::~IdTableModel()
    {
    }
}

