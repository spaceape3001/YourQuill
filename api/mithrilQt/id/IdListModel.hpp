////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QAbstractListModel>
#include <mithril/id/Id.hpp>
#include <mithril/id/IdFilter.hpp>
#include <mithril/id/IdProvider.hpp>
#include <mithrilQt/id/IdColumn.hpp>
#include <mithrilQt/id/IdModel.hpp>

namespace yq::mithril {
    class IdListModel : public QAbstractListModel, public IdModel {
        Q_OBJECT
    public:
        IdListModel(IdProvider::UPtr, IdColumn::UPtr, QObject*parent=nullptr);
        IdListModel(IdProvider::UPtr, IdColumn::UPtr, std::vector<IdFilter::UPtr>&&, QObject*parent=nullptr);
        ~IdListModel();
        
        QVariant        data(const QModelIndex&, int role) const override;
        Qt::ItemFlags   flags(const QModelIndex&) const override;
        int             rowCount(const QModelIndex&) const override;
        bool            setData(const QModelIndex&, const QVariant&, int ) override;
        
    public slots:
        void            reload();
        
    private:
        IdColumn::UPtr              m_column;
        
        void    _load();
    };
}
