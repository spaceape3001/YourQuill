////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithril/id/Id.hpp>
#include <yq/core/Compare.hpp>

#include <QVariant>
#include <functional>
#include <optional>

namespace yq::gluon {
    class Delegate;
};

namespace yq::mithril {
    using ::yq::gluon::Delegate;
    
    //! Generic column model (used by the other models)
    struct IdColumn {
        using UPtr      = std::unique_ptr<const IdColumn>;

        using VariantFN     = std::function<QVariant(Id)>;
        using AddFN         = std::function<Id(const QVariant&)>;
        using SetFN         = std::function<std::error_code(Id, const QVariant&)>;
        using DropIdFN      = std::function<std::error_code(Id, std::span<const Id>)>;
        using DropFN        = std::function<std::error_code(Id, const QVariant&)>;
        using MimeListFN    = std::function<QStringList(Id)>;
        using DelegateFN    = std::function<Delegate*()>;
        using CompareFN     = std::function<Compare(Id,Id)>;

        AddFN           fnAdd;              //!< Call if column is add-aware
        VariantFN       fnDecoration;       //!< Decoration data
        VariantFN       fnDisplay;          //!< Display data (mandatory)
        DelegateFN      fnDelegate;         //!< Delegate creator (can be non-existent)
        DropIdFN        fnDropId;           //!< Drop Id data here
        DropFN          fnDrop;             //!< Drop other data here
        MimeListFN      fnDropMime;         //!< Type of mimes allowed
        VariantFN       fnEdit;             //!< Edit data expected
        SetFN           fnSet;              //!< May work
        VariantFN       fnSort;             //!< Overrides display
        CompareFN       fnCompare;          //!< Comparison of two things, overrides fnSort

        QVariant        addMsg;             //!< Add message
        QVariant        label;              //!< Header label
        QVariant        defVal;             //!< Value if the ID is bad
        unsigned int    number  = 0;        //!< Column number, tracked by IdModel
        int             qtType  = 0;        //!< Qt data type

        IdColumn();
        ~IdColumn();

        using CreateFN      = IdColumn (*)(ColOpts);
        
        static bool                     hasColumn(IdTypeId, Column);
        static std::optional<IdColumn>  create(IdTypeId, Column, ColOpts opts={});

        static void                     declare(IdTypeId, Column, CreateFN);
        template <IdType S>
        static void                     declare(Column col, CreateFN fn)
        {
            declare(S::ID, col, fn);
        }
        
        
        static ColumnSpec               defaultList(IdTypeId);
        
        template <IdType S>
        static ColumnSpec               defaultList()
        {
            return defaultList(S::ID);
        }
        
        static std::span<const ColumnSpec>  defaultTable(IdTypeId);
        
        template <IdType S>
        static std::span<const ColumnSpec>  defaultTable()
        {
            return defaultTable(S::ID);
        }
        
        static void                     set_defaultList(IdTypeId, ColumnSpec);
        static void                     set_defaultTable(IdTypeId, std::initializer_list<ColumnSpec>);

        template <IdType S>
        static void                     set_defaultList(ColumnSpec spec)
        {
            set_defaultList(S::ID, spec);
        }
        
        template <IdType S>
        static void                     set_defaultTable(std::initializer_list<ColumnSpec> spec)
        {
            set_defaultTable(S::ID, spec);
        }
        
        struct Repo;
        struct Key;
        static Repo& repo();
    };
}
