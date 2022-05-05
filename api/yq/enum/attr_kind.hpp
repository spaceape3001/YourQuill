////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <yq/meta/InfoBinder.hpp>
#include <yq/type/Enum.hpp>

namespace yq {

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
}

YQ_TYPE_DECLARE(yq::AttrKind)
