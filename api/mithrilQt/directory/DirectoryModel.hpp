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
    
        DirectoryModel(Type t, QObject* parent=nullptr) : DirectoryModel(t, ALL, parent) {}
        DirectoryModel(Type t, all_t, QObject* parent=nullptr);
        ~DirectoryModel();
        
    private:
        DirectoryModel(Type t, Directory, IdProvider&&, QObject*parent);
    };
}
