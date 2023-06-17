////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "IdTableModel.hpp"

namespace yq::mithril {
    IdTableModel::IdTableModel(IdProvider::UPtr p, std::vector<IdColumn::UPtr>&&cols, QObject*parent) :
        QAbstractTableModel(parent), IdModel(std::move(p)), m_columns(std::move(cols))
    {
        _load();
    }
    
    IdTableModel::IdTableModel(IdProvider::UPtr p, std::vector<IdColumn::UPtr>&&cols, std::vector<IdFilter::UPtr>&&filters, QObject*parent) :
        QAbstractTableModel(parent), IdModel(std::move(p), std::move(filters)), m_columns(std::move(cols))
    {
    }
        
    IdTableModel::~IdTableModel()
    {
    }

    void            IdTableModel::_load()
    {
        m_data  = _fetch();
    }

    int             IdTableModel::columnCount(const QModelIndex&) const 
    {
        return m_columns.size();
    }
    
    QVariant        IdTableModel::data(const QModelIndex&idx, int role) const 
    {
        if(role == Qt::DisplayRole)
            return idx.row() + idx.column() + 1;
        return QVariant();
    }
    
    Qt::ItemFlags   IdTableModel::flags(const QModelIndex&idx) const 
    {
        return QAbstractTableModel::flags(idx);
    }
    
    void            IdTableModel::reload()
    {
        beginResetModel();
        _load();
        endResetModel();
    }
    
    int             IdTableModel::rowCount(const QModelIndex&) const 
    {
        return m_data.size();
    }
    
    bool            IdTableModel::setData(const QModelIndex&, const QVariant&, int ) 
    {
        return false;
    }

}

