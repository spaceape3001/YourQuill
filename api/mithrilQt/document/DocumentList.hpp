////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdList.hpp>

namespace yq::mithril {
    class DocumentModel;
    class DocumentList : public IdListT<Document> {
        Q_OBJECT
    public:
        static constexpr const Column   defColumn   = Column::Key;
    
        DocumentList(all_t, QWidget*parent=nullptr);
        DocumentList(all_t, Column col, QWidget*parent=nullptr);
        ~DocumentList();
        
        DocumentModel*          model();
        const DocumentModel*    model() const;
    };
}
