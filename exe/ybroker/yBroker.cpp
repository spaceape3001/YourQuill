////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <stdint.h>
#include <string>
#include "util/Logging.hpp"
#include "util/String.hpp"
#include "util/Vector.hpp"

#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <sys/stat.h>

#include <iostream>

#include <chrono>


/*
    The Broker is meant to be an intermediary for coordination
*/

struct WorkspaceInfo;

struct AppInfo {
    pid_t           pid;
    WorkspaceInfo*  wksp;
    std::string     path;
};

struct WorkspaceInfo {
    uint32_t                id;
    std::string             name, path, temp;
    pid_t                   updater, server;
    std::vector<AppInfo*>   apps;
};

enum class Quit {
    No      = 0,
    Stop,
    Restart
};

Quit        gQuit = Quit::No;
String      gTmpRoot;
String      gLogDir;
String      gIpcDir;
String      gPidDir;
String      gCacheDir;

void    sigQuit(int)
{
    gQuit   = Quit::Stop;
}

void    sigRestart(int)
{
    gQuit   = Quit::Restart;
}


#include "bDirectory.ipp"


int execMain(int argc, char* argv[])
{
    log_to_std_error(LogPriority::Info);
    if(!makeDirectories()){
        yFatal() << "Cannot create temporary directories under: " << gTmpRoot;
        return -1;
    }
    makeLogFile();
    
    yNotice() << "Starting up the broker!";
    

    int i   = 0;
    while(gQuit == Quit::No){
        std::chrono::steady_clock::now();
        ++i;
    }
    return 0;
}


int main(int argc, char* argv[])
{
    signal(SIGQUIT, sigQuit);
    signal(SIGTERM, sigQuit);
    signal(SIGHUP, sigRestart);

    int r = execMain(argc, argv);
    if((r == 0) && (gQuit == Quit::Restart)){
        yNotice() << "Restart requested.";
        Vector<char*>   args(argv, argv+argc);
        args << (char*) nullptr;
        execv(argv[0], args.data());
    }
    return r;
}

