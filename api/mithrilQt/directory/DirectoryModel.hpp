////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdModel.hpp>
#include <mithril/directory/Directory.hpp>
#include <mithrilQt/preamble.hpp>

namespace yq::mithril {
    class DirectoryModel : public IdModelT<Directory> {
        Q_OBJECT
    public:
    
        static std::optional<IdColumn>     resolve(Column, ColOpts opts=ColOpts());

        DirectoryModel(Type t, QObject* parent=nullptr) : DirectoryModel(t, ALL, parent) {}
        DirectoryModel(Type t, all_t, QObject* parent=nullptr);
        ~DirectoryModel();
        
        using IdModelT<Directory>::addColumn;
        void    addColumn(Column, ColOpts opts=ColOpts());
        void    addColumns(std::span<const Column> columns);

        using IdModelT<Directory>::setColumn;
        void    setColumn(Column, ColOpts opts=ColOpts());
        
    private:
        DirectoryModel(Type t, Directory, IdProvider&&, QObject*parent);
    };
}
