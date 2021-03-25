////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <meta/MetaValue.hpp>
#include <util/Enum.hpp>

/*! \enum AttrKind
    \brief Kind of attribute

    This represents the general "Kind" of the attribute, not the specific.
*/
YQ_ENUM(AttrKind, ,
    Value,          //!< Attibute is a simple "value" ... nothing to see, please move along
    State,          //!< Attribute is a state (implict atom) with start/stop
    Atom,           //!< This attribute is creating an atom (sub-to the current attom)
    Edge            //!< This attributes represents an edge (also an atom) to something else
)

MV_DECLARE(AttrKind)

