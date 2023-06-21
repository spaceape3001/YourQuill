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
    
        static std::optional<IdColumn>     resolve(Column, ColOpts opts=ColOpts());

        FieldModel(Type t, QObject* parent=nullptr) : FieldModel(t, ALL, parent) {}
        FieldModel(Type t, all_t, QObject* parent=nullptr);
        ~FieldModel();
        
        using IdModelT<Field>::addColumn;
        void    addColumn(Column, ColOpts opts=ColOpts());
        void    addColumns(std::span<const Column> columns);

        using IdModelT<Field>::setColumn;
        void    setColumn(Column, ColOpts opts=ColOpts());
        
    private:
        FieldModel(Type t, Field, IdProvider&&, QObject*parent);
    };
}
