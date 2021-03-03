////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Delegate.hpp"
#include <meta/ObjectImpl.hpp>

template <typename D> 
class TypedMetaDelegate : public TypedMetaObject<D> {
public:
    TypedMetaDelegate(const char* f) : TypedMetaObject<D>(f)
    {
        //MetaDelegate::m_value   = &metaValue<typename D::DataType>();
        MetaDelegate::m_qt      = qMetaTypeId<typename D::DataType>();
    }
};
