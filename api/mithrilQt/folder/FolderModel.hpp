////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdModel.hpp>
#include <mithril/folder/Folder.hpp>
#include <mithrilQt/column.hpp>

namespace yq::mithril {
    class FolderModel : public IdModelT<Folder> {
        Q_OBJECT
    public:
    
        static std::span<const Column>  defColumns();
    
        FolderModel(Type t) : FolderModel(t, ALL) {}
        FolderModel(Type t, all_t, std::span<const Column> cols=defColumns(), QObject* parent=nullptr);
        ~FolderModel();
        
        using IdModelT<Folder>::addColumn;
        void    addColumn(Column);
        
    private:
        FolderModel(Type t, Folder, IdProvider&&, std::span<const Column> cols, QObject*parent);
    };
}
