////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/id/Id.hpp>
#include <meta/Generator.hpp>

namespace yq::mithril {
    class IdProvider {
    public:
        using Factory           = Generator<const IdProvider*>;
        virtual std::vector<Id>     fetch() const = 0;
        virtual ~IdProvider(){}
    };
}
