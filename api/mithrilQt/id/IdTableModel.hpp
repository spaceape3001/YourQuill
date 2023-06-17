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
        IdTableModel(Id, IdProvider::UPtr, QObject*parent=nullptr);
        ~IdTableModel();
    
    protected:
        void    setColumns(std::vector<IdColumn>&&);
        void    addColumn(IdColumn&&);
        void    addColumn(size_t before, IdColumn&&);
    };
}

