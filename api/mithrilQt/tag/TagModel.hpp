////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdModel.hpp>
#include <mithril/tag/Tag.hpp>
#include <mithrilQt/preamble.hpp>
#include <optional>

namespace yq::mithril {
    class TagModel : public IdModelT<Tag> {
        Q_OBJECT
    public:
    
        static std::optional<IdColumn>     resolve(Column, ColOpts opts=ColOpts());
    
        TagModel(Type t, QObject* parent=nullptr) : TagModel(t, ALL, parent) {}
        TagModel(Type t, all_t, QObject* parent=nullptr);
        ~TagModel();
        
        using IdModelT<Tag>::addColumn;
        void    addColumn(Column, ColOpts opts=ColOpts());
        void    addColumns(std::span<const Column> columns);
        
        using IdModelT<Tag>::setColumn;
        void    setColumn(Column, ColOpts opts=ColOpts());
        
    private:
        TagModel(Type t, Tag, IdProvider&&, QObject*parent);
    };
}
