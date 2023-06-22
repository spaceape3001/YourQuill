////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdModel.hpp>
#include <mithril/field/Field.hpp>
#include <mithrilQt/preamble.hpp>

namespace yq::mithril {
    class FieldModel : public IdModelT<Field> {
        Q_OBJECT
    public:
    
        FieldModel(Type t, QObject* parent=nullptr) : FieldModel(t, ALL, parent) {}
        FieldModel(Type t, all_t, QObject* parent=nullptr);
        ~FieldModel();
        
    private:
        FieldModel(Type t, Field, IdProvider&&, QObject*parent);
    };
}
