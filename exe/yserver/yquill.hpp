////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq-toolbox/container/Vector.hpp>
#include <thread>

enum class Quit {
    No      = 0,
    Stop,
    Restart
};

extern volatile Quit   gQuit;

void    run_scanner(yq::Vector<std::thread>&);
void    run_server(yq::Vector<std::thread>&);

        //  Stage 1 -- only scans, creates the directories/fragments in the database
void    stage1_scan();

        //  Stage 2 -- call stage 2 handlers (before import of stage 3)
void    stage2_unimport();

        //  Stage 3 -- call the document startup handlers
void    stage3_documents();

        //  Stage 4 -- finalize before run
void    stage4_finalize();


