////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/Id.hpp>
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
    
        using VariantFN     = std::function<QVariant(uint64_t)>;
        using AddFN         = std::function<uint64_t(QVariant)>;
        using SetFN         = std::function<std::error_code(uint64_t, QVariant)>;
        using DropFN        = std::function<std::error_code(uint64_t, std::span<const uint64_t>)>;
        using DelegateFN    = std::function<Delegate*()>;
        
        using PublishFN     = std::function<IdColumn*()>;

        AddFN           fnAdd;
        VariantFN       fnDecoration;
        VariantFN       fnDisplayData;
        DelegateFN      fnDelegate;
        DropFN          fnDrop;
        VariantFN       fnEditData;
        SetFN           fnSet;
        VariantFN       fnSortData;

        QVariant        addMsg;
        QVariant        defVal;
        QVariant        label;

        IdColumn();
        ~IdColumn();

        template <size_t N>
        static void         publish(const char(&s)[N], PublishFN &&fn)
        {
            publish_(std::string_view(s,N), std::move(fn));
        }

        static IdColumn*    create(std::string_view);

    private:
        static void         publish_(std::string_view, PublishFN&&);
    
        struct Repo;
        static Repo&    repo();
    };
}
