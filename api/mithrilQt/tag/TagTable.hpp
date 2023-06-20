////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/column.hpp>
#include <mithrilQt/id/IdTable.hpp>

namespace yq::mithril {
    class TagModel;
    class TagTable : public IdTableT<Tag> {
        Q_OBJECT
    public:
        TagTable(all_t, QWidget*parent=nullptr);
        TagTable(all_t, std::initializer_list<Column> columns, QWidget*parent=nullptr);
        TagTable(all_t, std::span<const Column> columns, QWidget*parent=nullptr);
        ~TagTable();
        
        TagModel*          model();
        const TagModel*    model() const;
    };
}
