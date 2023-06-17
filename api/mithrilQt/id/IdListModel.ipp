////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "IdListModel.hpp"

namespace yq::mithril {
    
    
    IdListModel::IdListModel(IdProvider::UPtr provider, IdColumn::UPtr column, QObject*parent) :
        QAbstractListModel(parent), IdModel(std::move(provider)), m_column(std::move(column))
    {
        _load();
    }
    
    IdListModel::IdListModel(IdProvider::UPtr provider, IdColumn::UPtr column, std::vector<IdFilter::UPtr>&& filters, QObject*parent) :
        QAbstractListModel(parent), IdModel(std::move(provider), std::move(filters)),  m_column(std::move(column))
    {
        _load();
    }
    
    IdListModel::~IdListModel()
    {
    }
    
    void    IdListModel::_load()
    {
        m_data  = _fetch();
    }


    QVariant        IdListModel::data(const QModelIndex&idx, int role) const
    {
        if(role == Qt::DisplayRole)
            return idx.row()+1;
        return QVariant();
    }
    
    Qt::ItemFlags   IdListModel::flags(const QModelIndex& idx) const
    {
        return QAbstractListModel::flags(idx);
    }

    void            IdListModel::reload()
    {
        beginResetModel();
        _load();
        endResetModel();
    }

    int             IdListModel::rowCount(const QModelIndex&) const 
    {
        return m_data.size();
    }
    
    bool            IdListModel::setData(const QModelIndex&, const QVariant&, int ) 
    {
        return false;
    }
}
