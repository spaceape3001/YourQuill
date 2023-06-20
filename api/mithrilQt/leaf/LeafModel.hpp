////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdModel.hpp>
#include <mithril/leaf/Leaf.hpp>
#include <mithrilQt/column.hpp>

namespace yq::mithril {
    class LeafModel : public IdModelT<Leaf> {
        Q_OBJECT
    public:
    
        static std::span<const Column>  defColumns();
    
        LeafModel(Type t) : LeafModel(t, ALL) {}
        LeafModel(Type t, all_t, std::span<const Column> cols=defColumns(), QObject* parent=nullptr);
        ~LeafModel();
        
        using IdModelT<Leaf>::addColumn;
        void    addColumn(Column);
        
    private:
        LeafModel(Type t, Leaf, IdProvider&&, std::span<const Column> cols, QObject*parent);
    };
}
