////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QAbstractTableModel>
#include <mithril/id/Id.hpp>
#include <mithril/id/IdFilter.hpp>
#include <mithril/id/IdProvider.hpp>
#include <mithrilQt/id/IdColumn.hpp>
#include <mithrilQt/id/IdModel.hpp>

namespace yq::mithril {
    class IdTableModel : public QAbstractTableModel, public IdModel {
        Q_OBJECT
    public:
        IdTableModel(IdProvider::UPtr, std::vector<IdColumn::UPtr>&&, QObject*parent=nullptr);
        IdTableModel(IdProvider::UPtr, std::vector<IdColumn::UPtr>&&, std::vector<IdFilter::UPtr>&&, QObject*parent=nullptr);
        ~IdTableModel();

        int             columnCount(const QModelIndex&) const override;
        QVariant        data(const QModelIndex&, int role) const override;
        Qt::ItemFlags   flags(const QModelIndex&) const override;
        int             rowCount(const QModelIndex&) const override;
        bool            setData(const QModelIndex&, const QVariant&, int ) override;

    public slots:
        void            reload();
        
    private:
        std::vector<IdColumn::UPtr>     m_columns;

        void            _load();

    };
}

