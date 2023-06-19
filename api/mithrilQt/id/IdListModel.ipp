////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "IdListModel.hpp"

namespace yq::mithril {
    
    
    IdListModel::IdListModel(Id i, IdProvider&& provider, QObject*parent) :
        IdModel(Type::List, i, std::move(provider), parent)
    {
    }
    
    IdListModel::~IdListModel()
    {
    }

}
