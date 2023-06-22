////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdModel.hpp>
#include <mithril/document/Document.hpp>
#include <mithrilQt/preamble.hpp>

namespace yq::mithril {
    class DocumentModel : public IdModelT<Document> {
        Q_OBJECT
    public:

        DocumentModel(Type t, QObject* parent=nullptr) : DocumentModel(t, ALL, parent) {}
        DocumentModel(Type t, all_t, QObject* parent=nullptr);
        ~DocumentModel();

    private:
        DocumentModel(Type t, Document, IdProvider&&, QObject*parent);
    };
}
