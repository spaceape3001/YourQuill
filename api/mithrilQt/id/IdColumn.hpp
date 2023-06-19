////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/id/Id.hpp>
#include <QVariant>
#include <functional>

namespace yq::gluon {
    class Delegate;
};

namespace yq::mithril {
    using ::yq::gluon::Delegate;
    
    //! Generic column model (used by the other models)
    class IdColumn {
    public:
    
        using UPtr      = std::unique_ptr<const IdColumn>;

        using VariantFN     = std::function<QVariant(Id)>;
        using AddFN         = std::function<Id(QVariant)>;
        using SetFN         = std::function<std::error_code(Id, QVariant)>;
        using DropFN        = std::function<std::error_code(Id, std::span<const Id>)>;
        using DelegateFN    = std::function<Delegate*()>;

        AddFN           fnAdd;
        VariantFN       fnDecoration;
        VariantFN       fnDisplay;
        DelegateFN      fnDelegate;
        DropFN          fnDrop;
        VariantFN       fnEdit;
        SetFN           fnSet;
        VariantFN       fnSort;

        QVariant        addMsg;
        QVariant        defVal;
        QVariant        label;
        unsigned int    number = 0;

        IdColumn();
        ~IdColumn();

    };
}
