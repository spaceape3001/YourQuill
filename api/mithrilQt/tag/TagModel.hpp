////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdModel.hpp>
#include <mithril/tag/Tag.hpp>
#include <mithrilQt/column.hpp>

namespace yq::mithril {
    class TagModel : public IdModelT<Tag> {
        Q_OBJECT
    public:
    
        static std::span<const Column>  defColumns();
    
        TagModel(Type t) : TagModel(t, ALL) {}
        TagModel(Type t, all_t, std::span<const Column> cols=defColumns(), QObject* parent=nullptr);
        ~TagModel();
        
        using IdModelT<Tag>::addColumn;
        void    addColumn(Column);
        
    private:
        TagModel(Type t, Tag, IdProvider&&, std::span<const Column> cols, QObject*parent);
    };
}
