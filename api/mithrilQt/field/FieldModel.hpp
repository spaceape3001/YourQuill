////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdModel.hpp>
#include <mithril/field/Field.hpp>
#include <mithrilQt/column.hpp>

namespace yq::mithril {
    class FieldModel : public IdModelT<Field> {
        Q_OBJECT
    public:
    
        FieldModel(Type t, QObject* parent=nullptr) : FieldModel(t, ALL, parent) {}
        FieldModel(Type t, all_t, QObject* parent=nullptr);
        ~FieldModel();
        
        using IdModelT<Field>::addColumn;
        void    addColumn(Column);
        void    addColumns(std::span<const Column> columns);
        
    private:
        FieldModel(Type t, Field, IdProvider&&, QObject*parent);
    };
}
