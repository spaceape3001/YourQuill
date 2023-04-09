////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <aether/kernel/preamble.hpp>

namespace yq {

    template <typename> class IDLock;

    /*! \brief Atom class in the cache

        This structure represents an atom classification in the cache
    */
    struct Class {
        static constexpr const char*    szExtension = "class";
        struct Info;
        struct File;
        struct Data;
        struct Rank;
        using Lock = IDLock<Class>;
        using SharedData = std::shared_ptr<Data>;
        using SharedFile = std::shared_ptr<File>;
        

        static bool less_key(Class a, Class b);
        static bool less_label(Class a, Class b);
        static bool less_name(Class a, Class b);

        uint64_t  id  = 0ULL;
        constexpr auto    operator<=>(const Class&rhs) const noexcept = default;
        constexpr operator uint64_t() const noexcept { return id != 0ULL; }
    };

    struct Class::Rank {
        Class       cls;
        int64_t     rank    = 0;
        constexpr auto    operator<=>(const Rank&rhs) const noexcept = default;
    };

    using ClassFragDoc      = std::pair<Fragment, Class::SharedFile>;
}

YQ_TYPE_DECLARE(yq::Class)
