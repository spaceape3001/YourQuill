////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/id/Id.hpp>
#include <meta/Generator.hpp>

namespace yq::mithril {
    class IdFilter {
    public:
        using Factory           = Generator<const IdFilter*>;
        virtual bool            accept(Id) const = 0;
        virtual ~IdFilter(){}
    };
}
