////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdModel.hpp>
#include <mithril/root/Root.hpp>
#include <mithrilQt/preamble.hpp>

namespace yq::mithril {
    class RootModel : public IdModelT<Root> {
        Q_OBJECT
    public:

        static std::optional<IdColumn>     resolve(Column, ColOpts opts=ColOpts());
    
        RootModel(Type t, QObject* parent=nullptr) : RootModel(t, ALL, parent) {}
        RootModel(Type t, all_t, QObject* parent=nullptr);
        ~RootModel();
        
        using IdModelT<Root>::addColumn;
        void    addColumn(Column, ColOpts opts=ColOpts());
        void    addColumns(std::span<const Column> columns);

        using IdModelT<Root>::setColumn;
        void    setColumn(Column, ColOpts opts=ColOpts());
        
    private:
        RootModel(Type t, Root, IdProvider&&, QObject*parent);
    };
}
