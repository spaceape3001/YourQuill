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
        IdItemModel(Id, IdProvider&&, TreeDetector&&, ProviderGenerator&&, QObject*parent=nullptr);
        ~IdItemModel();
    };


}
