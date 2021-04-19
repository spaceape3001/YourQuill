////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <stdint.h>
#include <string>

#include "ipc/DirWatcher.hpp"
#include "ipc/ipcSocket.hpp"
#include "util/Logging.hpp"
#include "util/String.hpp"
#include "util/Vector.hpp"

#include <poll.h>
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

class BSocket : public ipc::Socket {
public:
    BSocket() : ipc::Socket( gIpcDir + "/broker", true) {}

    void    rxPrint(const std::string_view& s) override
    {
        std::cout << "rx> " << s << "\n";
    }
    
    void    rxRestart() override
    {
        gQuit       = Quit::Restart;
    }
    
    void    rxShutdown() override 
    {
        gQuit       = Quit::Stop;
    }
};


class BWatcher : public ipc::DirWatcher {
public:
    BWatcher()
    {
    }
};

#include "bDirectory.ipp"


int execMain(int argc, char* argv[])
{
    log_to_std_error(LogPriority::Info);
    if(!makeDirectories()){
        yCritical() << "Cannot create temporary directories under: " << gTmpRoot;
        return -1;
    }
    makeLogFile();
    
    yNotice() << "Starting up the broker!";
    
    BSocket         sock;
    if(sock.fd() == 1){
        yCritical() << "Cannot create inbound pipe!";
        return -1;
    }
    
    BWatcher      dwatch;
    if(dwatch.fd()  == 1){
        yCritical() << "Cannot create inotify instance";
        return -1;
    }
    
    pollfd  fds[2] = {{ sock.fd(), POLLIN, 0 }, { dwatch.fd(), POLLIN, 0 }};
    while(gQuit == Quit::No){
        int pn  = poll(fds, 2, 100);
        if(pn == -1){
            yCritical() << "Error in polling";
            return -1;
        }
        if(pn > 0){
            if(fds[0].revents & POLLIN)
                sock.process();
            if(fds[1].revents & POLLIN)
                dwatch.process();
        }
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

