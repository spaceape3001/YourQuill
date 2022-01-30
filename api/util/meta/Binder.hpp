#pragma once
#include <util/preamble.hpp>

namespace yq {
    namespace meta {
    
        struct False {
            static constexpr const bool Defined     = false;
            static constexpr const bool IsCompound  = false;
            static constexpr const bool IsType      = false;
            static constexpr const bool IsObject    = false;
            static constexpr const bool IsValue     = false;
            static constexpr const bool IsStructure = false;
        };
    
        template <typename T>
        struct Binder : public False {
        };
        
        template <typename T>
        static constexpr const bool     defined     = Binder<T>::Defined;
    
        template <typename T>
        static constexpr const bool     is_type     = Binder<T>::IsType;

        template <typename T>
        static constexpr const bool     is_object   = Binder<T>::IsObject;

        template <typename T>
        static constexpr const bool     is_compound = Binder<T>::IsCompound;

        template <typename T>
        static constexpr const bool     is_value   = Binder<T>::IsValue;
        
        template <typename T>
        static constexpr const bool     is_struct  = Binder<T>::IsStructure;

        const TypeInfo&        invalid();
        const TypeInfo&        variant();

        template <>
        struct Binder<void> : public False {
            static constexpr const bool Defined     = true;
            static constexpr const bool IsType      = true;     
            static const TypeInfo&  bind() { return invalid(); }
        };

        template <>
        struct Binder<Variant> : public False {
            static constexpr const bool Defined     = true;
            static constexpr const bool IsType      = true;     
            static const TypeInfo&  bind() { return variant(); }
        };
        
        template <typename T>
        auto edit()
        {
            return Binder<T>::edit();
        }
     
    }
    
    template <typename T>
    auto    metaOf()
    {
        static_assert(meta::defined<T>, "Must be meta-declared!");
        return meta::Binder<T>::bind();
    }

}


/*! \brief Declares an object type

    \note   MUST BE USED AT GLOBAL SCOPE (NO NAMESPACES)
*/
#define MO_DECLARE(name)                                        \
    namespace yq { namespace meta {                             \
        template <>                                             \
        struct Binder(name) : public False {                    \
            static constexpr const bool Defined     = true;     \
            static constexpr const bool IsObject    = true;     \
            static constexpr const bool IsCompound  = true;     \
            static const ObjectInfo&        bind();             \
            static ObjectInfo&              edit();             \
        };                                                      \
    } }


/*! \brief Declares a meta structure

    \note   MUST BE USED AT GLOBAL SCOPE (NO NAMESPACES)
*/
#define MS_DECLARE(name)                                        \
    namespace yq { namespace meta {                             \
        template <>                                             \
        struct Binder<name> : public False {                    \
            static constexpr const bool Defined     = true;     \
            static constexpr const bool IsType      = true;     \
            static constexpr const bool IsStructure = true;     \
            static constexpr const bool IsCompound  = true;     \
            static const TypeInfo&          bind();             \
            static TypeInfo&                edit();             \
        };                                                      \
    } }

/*! \brief Declares a meta type

    \note   MUST BE USED AT GLOBAL SCOPE (NO NAMESPACES)
*/
#define MT_DECLARE(name)                                        \
    namespace yq { namespace meta {                             \
        template <>                                             \
        struct Binder<name> : public False {                    \
            static constexpr const bool Defined     = true;     \
            static constexpr const bool IsType      = true;     \
            static constexpr const bool IsCompound  = true;     \
            static const TypeInfo&          bind();             \
            static TypeInfo&                edit();             \
        };                                                      \
    } }

/*! \brief Declares a meta value

    \note   MUST BE USED AT GLOBAL SCOPE (NO NAMESPACES)
*/
#define MV_DECLARE(name)                                        \
    namespace yq { namespace meta {                             \
        template <>                                             \
        struct Binder<name> : False {                           \
            static constexpr const bool Defined     = true;     \
            static constexpr const bool IsType      = true;     \
            static constexpr const bool IsValue     = true;     \
            static constexpr const bool IsCompound  = true;     \
            static const ValueInfo&          bind();            \
            static ValueInfo&                edit();            \
        };                                                      \
    } }


MV_DECLARE(String)
MV_DECLARE(bool)
MV_DECLARE(double)
MV_DECLARE(float)
MV_DECLARE(int8_t)
MV_DECLARE(int16_t)
MV_DECLARE(int32_t)
MV_DECLARE(int64_t)
MV_DECLARE(uint8_t)
MV_DECLARE(uint16_t)
MV_DECLARE(uint32_t)
MV_DECLARE(uint64_t)
