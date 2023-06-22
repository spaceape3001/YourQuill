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
    
        FragmentModel(Type t, QObject* parent=nullptr) : FragmentModel(t, ALL, parent) {}
        FragmentModel(Type t, all_t, QObject* parent=nullptr);
        ~FragmentModel();

    private:
        FragmentModel(Type t, Fragment, IdProvider&&, QObject*parent);
    };
}
