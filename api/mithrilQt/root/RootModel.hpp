////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdModel.hpp>
#include <mithril/root/Root.hpp>
#include <mithrilQt/column.hpp>

namespace yq::mithril {
    class RootModel : public IdModelT<Root> {
        Q_OBJECT
    public:
    
        RootModel(Type t, QObject* parent=nullptr) : RootModel(t, ALL, parent) {}
        RootModel(Type t, all_t, QObject* parent=nullptr);
        ~RootModel();
        
        using IdModelT<Root>::addColumn;
        void    addColumn(Column);
        void    addColumns(std::span<const Column> columns);
        
    private:
        RootModel(Type t, Root, IdProvider&&, QObject*parent);
    };
}
