////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdModel.hpp>

namespace yq::mithril {
    class IdTableModel : public IdModel {
        Q_OBJECT
    public:
        IdTableModel(Id, IdProvider&&, QObject*parent=nullptr);
        ~IdTableModel();
    
    };
}

