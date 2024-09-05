////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <0/basic/preamble.hpp>
#include <yq-toolbox/meta/InfoBinder.hpp>
#include <unordered_map>

namespace yq {
    class TypeInfo;
}


namespace yq::doodler {

    struct ID {
        using id_t      = uint32_t;
        using Remapper  = std::unordered_map<id_t,id_t>;
        id_t    id  = {};
        constexpr operator id_t () const noexcept { return id; }
        constexpr auto operator<=>(const ID&) const noexcept = default;
    };
    
    class DObject;
    class Project;
    class Remapper;


    class Association;
    class Constraint;
    class Camera;
    class Light;
    class Motion;
    
    class Py;
    class Py1;
    class Py2;
    class Py3;
    class Py4;
    
    class Space;
    
    
}

YQ_TYPE_DECLARE(yq::doodler::ID)
