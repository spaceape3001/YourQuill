////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdModel.hpp>
#include <mithril/leaf/Leaf.hpp>
#include <mithrilQt/preamble.hpp>

namespace yq::mithril {
    class LeafModel : public IdModelT<Leaf> {
        Q_OBJECT
    public:
    
    
        LeafModel(Type t, QObject* parent=nullptr) : LeafModel(t, ALL, parent) {}
        LeafModel(Type t, all_t, QObject* parent=nullptr);
        ~LeafModel();
        
        using IdModelT<Leaf>::addColumn;
        void    addColumn(Column);
        void    addColumns(std::span<const Column> columns);
        
    private:
        LeafModel(Type t, Leaf, IdProvider&&, QObject*parent);
    };
}
