////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdModel.hpp>
#include <mithril/class/Class.hpp>
#include <mithrilQt/column.hpp>

namespace yq::mithril {
    class ClassModel : public IdModelT<Class> {
        Q_OBJECT
    public:
    
        static std::span<const Column>  defColumns();
    
        ClassModel(Type t) : ClassModel(t, ALL) {}
        ClassModel(Type t, all_t, std::span<const Column> cols=defColumns(), QObject* parent=nullptr);
        ~ClassModel();
        
        using IdModelT<Class>::addColumn;
        void    addColumn(Column);
        
    private:
        ClassModel(Type t, Class, IdProvider&&, std::span<const Column> cols, QObject*parent);
    };
}
