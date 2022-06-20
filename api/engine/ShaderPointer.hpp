////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/Ref.hpp>
#include <string>
#include <variant>

namespace yq {
    class Shader;
    using ShaderPtr     = Ref<const Shader>;
    using ShaderSpec    = std::variant<std::monostate, std::string, ShaderPtr>;
}
