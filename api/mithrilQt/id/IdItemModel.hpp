////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdModel.hpp>

namespace yq::mithril {
    class IdItemModel : public IdModel {
        Q_OBJECT
    public:
        IdItemModel(Id, IdProvider::UPtr, TreeDetector&&, ProviderGenerator&&, QObject*parent=nullptr);
        ~IdItemModel();
    
    protected:
        void    setColumns(std::vector<IdColumn>&&);
        void    addColumn(IdColumn&&);
        void    addColumn(size_t before, IdColumn&&);
    };


}
