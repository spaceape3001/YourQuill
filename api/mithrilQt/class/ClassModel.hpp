////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdModel.hpp>
#include <mithril/class/Class.hpp>
#include <mithrilQt/preamble.hpp>

namespace yq::mithril {
    class ClassModel : public IdModelT<Class> {
        Q_OBJECT
    public:
    
        //static std::optional<IdColumn>     resolve(Column, ColOpts opts=ColOpts());

        ClassModel(Type t, QObject* parent=nullptr) : ClassModel(t, ALL, parent) {}
        ClassModel(Type t, all_t, QObject* parent=nullptr);
        ~ClassModel();
        
        //using IdModelT<Class>::addColumn;
        //void    addColumn(Column, ColOpts opts=ColOpts());
        //void    addColumns(std::span<const Column> columns);
        
        //using IdModelT<Class>::setColumn;
        //void    setColumn(Column, ColOpts opts=ColOpts());

    private:
        ClassModel(Type t, Class, IdProvider&&, QObject*parent);
    };
}
