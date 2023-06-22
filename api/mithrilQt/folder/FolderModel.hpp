////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdModel.hpp>
#include <mithril/folder/Folder.hpp>
#include <mithrilQt/preamble.hpp>

namespace yq::mithril {
    class FolderModel : public IdModelT<Folder> {
        Q_OBJECT
    public:
        FolderModel(Type t, QObject* parent=nullptr) : FolderModel(t, ALL, parent) {}
        FolderModel(Type t, all_t, QObject* parent=nullptr);
        ~FolderModel();
        
    private:
        FolderModel(Type t, Folder, IdProvider&&, QObject*parent);
    };
}
