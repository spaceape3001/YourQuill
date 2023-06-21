////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdModel.hpp>
#include <mithril/tag/Tag.hpp>
#include <mithrilQt/preamble.hpp>

namespace yq::mithril {
    class TagModel : public IdModelT<Tag> {
        Q_OBJECT
    public:
    
        TagModel(Type t, QObject* parent=nullptr) : TagModel(t, ALL, parent) {}
        TagModel(Type t, all_t, QObject* parent=nullptr);
        ~TagModel();
        
        using IdModelT<Tag>::addColumn;
        void    addColumn(Column);
        void    addColumns(std::span<const Column> columns);
        
    private:
        TagModel(Type t, Tag, IdProvider&&, QObject*parent);
    };
}
