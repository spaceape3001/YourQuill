#pragma once

#include <filesystem>

namespace yq {
    bool        load_plugin(const std::filesystem::path&);
    size_t      load_plugin_dir(const std::filesystem::path&);
}