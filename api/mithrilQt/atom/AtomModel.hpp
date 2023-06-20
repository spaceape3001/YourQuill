////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdModel.hpp>
#include <mithril/atom/Atom.hpp>
#include <mithrilQt/column.hpp>

namespace yq::mithril {
    class AtomModel : public IdModelT<Atom> {
        Q_OBJECT
    public:
        AtomModel(Type t, QObject*parent=nullptr) : AtomModel(t, ALL, parent) {}
        AtomModel(Type t, all_t, QObject* parent=nullptr);
        ~AtomModel();
        
        using IdModelT<Atom>::addColumn;
        void    addColumn(Column);
        
        void    addColumns(std::span<const Column> columns);
        
    private:
        AtomModel(Type t, Atom, IdProvider&&, QObject*parent);
    };
}
