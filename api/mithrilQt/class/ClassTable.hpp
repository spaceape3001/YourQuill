////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdTable.hpp>

namespace yq::mithril {
    class ClassModel;
    class ClassTable : public IdTableT<Class> {
        Q_OBJECT
    public:
        static std::span<const Column>  defColumns();

        ClassTable(all_t, QWidget*parent=nullptr);
        ClassTable(all_t, std::initializer_list<Column> columns, QWidget*parent=nullptr);
        ClassTable(all_t, std::span<const Column> columns, QWidget*parent=nullptr);
        ~ClassTable();
        
        ClassModel*          model();
        const ClassModel*    model() const;
    };
}
