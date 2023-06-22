////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdModel.hpp>
#include <mithril/class/Class.hpp>
#include <mithrilQt/preamble.hpp>

namespace yq::mithril {
    class ClassModel : public IdModelT<Class> {
        Q_OBJECT
    public:
        ClassModel(Type t, QObject* parent=nullptr) : ClassModel(t, ALL, parent) {}
        ClassModel(Type t, all_t, QObject* parent=nullptr);
        ~ClassModel();

    private:
        ClassModel(Type t, Class, IdProvider&&, QObject*parent);
    };
}
