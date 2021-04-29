////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <util/String.hpp>
#include <util/Vector.hpp>

struct CommonDir {
    const char*     build;
    String          cache;
    String          ipc;
    String          ini;
    String          log;
    String          pid;
    Vector<String>  share;
    String          tmp;

    static bool     init();
};

extern const CommonDir&     gDir;

