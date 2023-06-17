////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/id/Id.hpp>
#include <meta/Generator.hpp>
#include <memory>

namespace yq::mithril {
    class IdFilter {
    public:
        using Factory           = Generator<const IdFilter*>;
        virtual bool            accept(Id) const = 0;
        virtual ~IdFilter(){}
        
        using UPtr      = std::unique_ptr<const IdFilter>;
    };
}
