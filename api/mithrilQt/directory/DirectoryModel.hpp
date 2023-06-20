////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdModel.hpp>
#include <mithril/directory/Directory.hpp>
#include <mithrilQt/column.hpp>

namespace yq::mithril {
    class DirectoryModel : public IdModelT<Directory> {
        Q_OBJECT
    public:
    
        DirectoryModel(Type t, QObject* parent=nullptr) : DirectoryModel(t, ALL, parent) {}
        DirectoryModel(Type t, all_t, QObject* parent=nullptr);
        ~DirectoryModel();
        
        using IdModelT<Directory>::addColumn;
        void    addColumn(Column);
        void    addColumns(std::span<const Column> columns);
        
    private:
        DirectoryModel(Type t, Directory, IdProvider&&, QObject*parent);
    };
}
