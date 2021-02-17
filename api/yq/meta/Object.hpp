////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "MetaFwd.hpp"
#include <type_traits>
#include "MetaObject.hpp"

#define MO_META(NewMeta)                                        \
public:                                                         \
    using Meta             = NewMeta;
    
#define MO_DECLARE(ObjectClass, BaseClass)                      \
public:                                                         \
    friend class TypedMetaObject<ObjectClass>;                  \
    static constexpr const char* szClassName    = #ObjectClass; \
    using This      = ObjectClass;                              \
    using Base      = BaseClass;                                \
    static const Meta&              static_meta();              \
    virtual const Meta&             meta() const;
    
#define MO_TYPED(NewTyped)                                      \
public:                                                         \
    template <typename T> using Typed = NewTyped<T>;

/*! \brief This is the root "meta-capable" object

    This complements the Variant, in that it's uber generic
*/
class Object {

    MO_TYPED(TypedMetaObject)
    MO_META(MetaObject)
    MO_DECLARE(Object, void)

public:
    virtual ~Object();
   
protected:
    Object();
};

/*! \brief Redfines the meta object class
    \note Use this *BEFORE* MO_DECLARE
*/
template <typename T>
const auto& metaObject()
{
    static_assert(std::is_base_of_v<Object,T>, "Not an object type!");
    return T::static_meta();
}
