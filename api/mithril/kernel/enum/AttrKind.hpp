////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <meta/InfoBinder.hpp>
#include <basic/Enum.hpp>

namespace yq {

    namespace mithril {
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
}

YQ_TYPE_DECLARE(yq::mithril::AttrKind)
