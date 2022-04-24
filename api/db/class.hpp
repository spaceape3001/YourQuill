////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <compare>
#include <cstdint>

namespace yq {

    template <typename> class IDLock;

    /*! \brief Atom class in the cache

        This structure represents an atom classification in the cache
    */
    struct Class {
        static constexpr const char*    szExtension = "cls";
        struct Info;
        struct File;
        struct Data;
        using Lock = IDLock<Class>;
        using SharedData = std::shared_ptr<Data>;
        using SharedFile = std::shared_ptr<File>;
        

        static bool less_key(Class a, Class b);
        static bool less_label(Class a, Class b);
        static bool less_name(Class a, Class b);

        uint64_t  id  = 0ULL;
        constexpr auto    operator<=>(const Class&rhs) const = default;
        constexpr operator bool() const { return id != 0ULL; }
    };
}
