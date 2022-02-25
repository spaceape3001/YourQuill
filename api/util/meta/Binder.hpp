#pragma once
#include <util/preamble.hpp>

namespace yq {
    template <typename T>
    struct InfoBinder {
        static constexpr const bool Defined     = false;
        static constexpr const bool IsCompound  = false;
        static constexpr const bool IsType      = false;
        static constexpr const bool IsObject    = false;
    };

    template <typename T>
    static constexpr const bool     is_defined_v     = InfoBinder<T>::Defined;
    
    template <typename T>
    static constexpr const bool     is_type_v     = InfoBinder<T>::IsType;

    template <typename T>
    static constexpr const bool     is_object_v   = InfoBinder<T>::IsObject;

    template <typename T>
    static constexpr const bool     is_compound_v = InfoBinder<T>::IsCompound;

    const TypeInfo&        invalid();
    const TypeInfo&        variant();


    template <>
    struct InfoBinder<void> {
        static constexpr const bool Defined     = true;
        static constexpr const bool IsType      = true;     
        static const TypeInfo&  bind() { return invalid(); }
    };

    template <>
    struct InfoBinder<Variant> {
        static constexpr const bool Defined     = true;
        static constexpr const bool IsType      = true;     
        static const TypeInfo&  bind() { return variant(); }
    };

    template <typename T>
    const auto&    meta()
    {
        static_assert(is_defined_v<T>, "Must be meta-declared!");
        return InfoBinder<T>::bind();
    }


    template <typename T>
    auto&          meta_edit()
    {
        static_assert(is_defined_v<T>, "Must be meta-declared!");
        return InfoBinder<T>::edit();
    }

    template <typename T>
    auto           meta_write()
    {
        auto&    e   = meta_edit<T>();
        return typename std::remove_cvref_t<decltype(e)>::template Writer<T>(e);
    }
}


/*! \brief Declares an object type

    \note   MUST BE USED AT GLOBAL SCOPE (NO NAMESPACES)
*/
#define MO_DECLARE(...)                                         \
    namespace yq {                                              \
        template <>                                             \
        struct InfoBinder(__VA_ARGS__) {                        \
            static constexpr const bool Defined         = true; \
            static constexpr const bool IsObject        = true; \
            static constexpr const bool IsCompound      = true; \
            static const ObjectInfo& bind() { return edit(); }  \
            static ObjectInfo&       edit();                    \
        };                                                      \
    }


/*! \brief Declares a meta type

    \note   MUST BE USED AT GLOBAL SCOPE (NO NAMESPACES)
*/
#define MT_DECLARE(...)                                         \
    namespace yq {                                              \
        template <>                                             \
        struct InfoBinder<__VA_ARGS__> {                        \
            static constexpr const bool Defined     = true;     \
            static constexpr const bool IsObject    = false;    \
            static constexpr const bool IsType      = true;     \
            static constexpr const bool IsCompound  = true;     \
            static const TypeInfo&   bind() { return edit(); }  \
            static TypeInfo&         edit();                    \
        };                                                      \
    } 


MT_DECLARE(std::string)
MT_DECLARE(bool)
MT_DECLARE(double)
MT_DECLARE(float)
MT_DECLARE(int8_t)
MT_DECLARE(int16_t)
MT_DECLARE(int32_t)
MT_DECLARE(int64_t)
MT_DECLARE(uint8_t)
MT_DECLARE(uint16_t)
MT_DECLARE(uint32_t)
MT_DECLARE(uint64_t)
