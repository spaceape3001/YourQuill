////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/enum/DataRole.hpp>

class Root;
template <typename E, typename V> class EnumMap;
using RootMap       = Map<QString,const Root*,IgCase>;
using RootVector    = Vector<const Root*>;
using RoleMap       = EnumMap<DataRole, const Root*>;
using RoleVecMap    = EnumMap<DataRole, Vector<const Root*>>;

