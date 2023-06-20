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
    
        static std::span<const Column>  defColumns();
    
        DirectoryModel(Type t) : DirectoryModel(t, ALL) {}
        DirectoryModel(Type t, all_t, std::span<const Column> cols=defColumns(), QObject* parent=nullptr);
        ~DirectoryModel();
        
        using IdModelT<Directory>::addColumn;
        void    addColumn(Column);
        
    private:
        DirectoryModel(Type t, Directory, IdProvider&&, std::span<const Column> cols, QObject*parent);
    };
}
