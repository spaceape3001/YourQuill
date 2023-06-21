////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdModel.hpp>
#include <mithril/fragment/Fragment.hpp>
#include <mithrilQt/preamble.hpp>

namespace yq::mithril {
    class FragmentModel : public IdModelT<Fragment> {
        Q_OBJECT
    public:
    
        static std::optional<IdColumn>     resolve(Column, ColOpts opts=ColOpts());

        FragmentModel(Type t, QObject* parent=nullptr) : FragmentModel(t, ALL, parent) {}
        FragmentModel(Type t, all_t, QObject* parent=nullptr);
        ~FragmentModel();
        
        using IdModelT<Fragment>::addColumn;
        void    addColumn(Column, ColOpts opts=ColOpts());
        void    addColumns(std::span<const Column> columns);
        
        using IdModelT<Fragment>::setColumn;
        void    setColumn(Column, ColOpts opts=ColOpts());

    private:
        FragmentModel(Type t, Fragment, IdProvider&&, QObject*parent);
    };
}
