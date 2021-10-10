////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <util/String.hpp>
#include <util/Vector.hpp>
#include <filesystem>

struct CommonDir {
    using fspath = std::filesystem::path;
    fspath          build;
    fspath          cache;
    fspath          ipc;
    fspath          ini;
    fspath          log;
    fspath          pid;
    Vector<fspath>  share;
    fspath          tmp;

    static bool     init();
};

extern const CommonDir&     gDir;

std::filesystem::path              shared(const std::filesystem::path&);
Vector<std::filesystem::path>      shared_all(const std::filesystem::path&);

