////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdModel.hpp>
#include <mithril/fragment/Fragment.hpp>
#include <mithrilQt/column.hpp>

namespace yq::mithril {
    class FragmentModel : public IdModelT<Fragment> {
        Q_OBJECT
    public:
    
        static std::span<const Column>  defColumns();
    
        FragmentModel(Type t) : FragmentModel(t, ALL) {}
        FragmentModel(Type t, all_t, std::span<const Column> cols=defColumns(), QObject* parent=nullptr);
        ~FragmentModel();
        
        using IdModelT<Fragment>::addColumn;
        void    addColumn(Column);
        
    private:
        FragmentModel(Type t, Fragment, IdProvider&&, std::span<const Column> cols, QObject*parent);
    };
}
