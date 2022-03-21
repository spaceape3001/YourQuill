#pragma once
#include <yq/preamble.hpp>

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
        auto&   e   = meta_edit<T>();
        return typename std::remove_cvref_t<decltype(e)>::template Writer<T>(&e);
    }
}


/*! \brief Declares an object type

    \note   MUST BE USED AT GLOBAL SCOPE (NO NAMESPACES)
*/
#define YQ_OBJECT_DECLARE(...)                                  \
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
#define YQ_TYPE_DECLARE(...)                                    \
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


YQ_TYPE_DECLARE(std::string)
YQ_TYPE_DECLARE(bool)
YQ_TYPE_DECLARE(double)
YQ_TYPE_DECLARE(float)
YQ_TYPE_DECLARE(int8_t)
YQ_TYPE_DECLARE(int16_t)
YQ_TYPE_DECLARE(int32_t)
YQ_TYPE_DECLARE(int64_t)
YQ_TYPE_DECLARE(uint8_t)
YQ_TYPE_DECLARE(uint16_t)
YQ_TYPE_DECLARE(uint32_t)
YQ_TYPE_DECLARE(uint64_t)
