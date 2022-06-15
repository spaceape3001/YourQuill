#pragma once

#include <glm/glm.hpp>


namespace yq {
    namespace engine {
        struct StdPushConstant {
            glm::mat4       matrix; // either full or render... depending on enumeration
        };
    }
}
