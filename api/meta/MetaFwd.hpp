////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "util/TypeFwd.hpp"

union DataBlock;
class Global;
class Meta;
class MetaAspect;
class MetaField;
class MetaFunction;
class MetaMethod;
class MetaObject;
class MetaProperty;
class MetaType;
class MetaValue;

class Variant;

template <typename T> struct TypedMetaObject;
using VariantMap            = Map<String,Variant,IgCase>;
using variant_r         = Result<Variant>;


/*! \brief "Intiialzies" the core

    This calls Delay Init's init_all()
*/
void meta_init();

/*! \brief Freezes the core

    This freezes/locks ALL of the registration in the system.  (It'll call init before it does so)
*/
void meta_freeze();

/*! \brief Registers a hook to be called during the final freeze process */
void meta_freeze_hook(void(*)());
