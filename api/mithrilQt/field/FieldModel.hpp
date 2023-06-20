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
    
        static std::span<const Column>  defColumns();
    
        FieldModel(Type t) : FieldModel(t, ALL) {}
        FieldModel(Type t, all_t, std::span<const Column> cols=defColumns(), QObject* parent=nullptr);
        ~FieldModel();
        
        using IdModelT<Field>::addColumn;
        void    addColumn(Column);
        
    private:
        FieldModel(Type t, Field, IdProvider&&, std::span<const Column> cols, QObject*parent);
    };
}
