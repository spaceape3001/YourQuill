////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <util/text/String.hpp>
#include <util/collection/Vector.hpp>

namespace yq {

    struct CommonDir {
        using fspath = std::filesystem::path;
        fspath          build;
        fspath          cache;      // global non-quill cache
        fspath          ipc;        // global non-quill IPC
        fspath          ini;        // global non-quill INI location
        fspath          log;        // global non-quill LOG file location
        fspath          pid;        // global non-quill PID location
        Vector<fspath>  share;      // SHARE path 9global)
        fspath          tmp;        // global temp

        static bool     init();
    };

    extern const CommonDir&     gDir;

    std::filesystem::path              shared(const std::filesystem::path&);
    Vector<std::filesystem::path>      shared_all(const std::filesystem::path&);

}
