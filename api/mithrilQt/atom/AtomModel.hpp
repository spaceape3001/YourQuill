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
    
        static std::span<const Column>  defColumns();
    
        AtomModel(Type t) : AtomModel(t, ALL) {}
        AtomModel(Type t, all_t, std::span<const Column> cols=defColumns(), QObject* parent=nullptr);
        ~AtomModel();
        
        using IdModelT<Atom>::addColumn;
        void    addColumn(Column);
        
    private:
        AtomModel(Type t, Atom, IdProvider&&, std::span<const Column> cols, QObject*parent);
    };
}
