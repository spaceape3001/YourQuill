////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdModel.hpp>

namespace yq::mithril {
    class IdListModel : public IdModel {
        Q_OBJECT
    public:
        IdListModel(Id, IdProvider::UPtr&&, QObject*parent=nullptr);
        ~IdListModel();
        
    protected:
        void    setColumn(IdColumn&&);
    };
}
