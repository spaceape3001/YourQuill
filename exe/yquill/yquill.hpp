////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/preamble.hpp>
#include <thread>

enum class Quit {
    No      = 0,
    Stop,
    Restart
};

extern volatile Quit   gQuit;

void        run_scanner(yq::Vector<std::thread>&);
void        run_server(yq::Vector<std::thread>&);
