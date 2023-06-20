////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/column.hpp>
#include <mithrilQt/id/IdTable.hpp>

namespace yq::mithril {
    class FieldModel;
    class FieldTable : public IdTableT<Field> {
        Q_OBJECT
    public:

        static std::span<const Column>  defColumns();
    
        FieldTable(all_t, QWidget*parent=nullptr);
        FieldTable(all_t, std::initializer_list<Column> columns, QWidget*parent=nullptr);
        FieldTable(all_t, std::span<const Column> columns, QWidget*parent=nullptr);
        ~FieldTable();
        
        FieldModel*          model();
        const FieldModel*    model() const;
    };
}
