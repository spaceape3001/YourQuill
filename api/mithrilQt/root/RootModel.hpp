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
    
        static std::span<const Column>  defColumns();
    
        RootModel(Type t) : RootModel(t, ALL) {}
        RootModel(Type t, all_t, std::span<const Column> cols=defColumns(), QObject* parent=nullptr);
        ~RootModel();
        
        using IdModelT<Root>::addColumn;
        void    addColumn(Column);
        
    private:
        RootModel(Type t, Root, IdProvider&&, std::span<const Column> cols, QObject*parent);
    };
}
