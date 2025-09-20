////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/document/Document.hpp>

namespace yq::mithril {

    /*! \brief Atom class in the cache

        This structure represents an atom classification in the cache
    */
    struct Class {
        static constexpr const IdTypeId ID          = 7;
        static constexpr const IdTypes  PARENTS     = Document::ID;
        static constexpr const char*    EXTENSION   = "cls";
        
        struct Info;
        struct File;
        struct Data;
        struct Rank;
        struct Diff;
        using SharedData = std::shared_ptr<Data>;
        using SharedFile = std::shared_ptr<File>;
        
        using Notify = Notifier<const Diff&>;
        

        static bool less_key(Class a, Class b);
        static bool less_label(Class a, Class b);
        static bool less_name(Class a, Class b);

        uint64_t  id  = 0ULL;
        constexpr auto    operator<=>(const Class&rhs) const noexcept = default;
        constexpr operator uint64_t() const noexcept { return id; }
        
        constexpr bool valid() const { return static_cast<bool>(id); }
    };

    struct Class::Rank {
        Class       cls;
        int64_t     rank    = 0;
        constexpr auto    operator<=>(const Rank&rhs) const noexcept = default;
    };

    using ClassFragDoc      = std::pair<Fragment, Class::SharedFile>;
}

YQ_TYPE_DECLARE(yq::mithril::Class)
YQ_TYPE_DECLARE(yq::mithril::ClassSet)
YQ_TYPE_DECLARE(yq::mithril::ClassVector)
