////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <stdint.h>
#include <string>

#include <db/CommonDirs.hpp>
#include <ipc/DirWatcher.hpp>
#include <ipc/PidFile.hpp>
#include <ipc/ipcSocket.hpp>

#include <util/CmdArgs.hpp>
#include <util/FileUtils.hpp>
#include <util/Logging.hpp>
#include <util/Safety.hpp>
#include <util/String.hpp>
#include <util/Vector.hpp>

#include <poll.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <sys/stat.h>

#include <fstream>
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
    BSocket() : ipc::Socket( gDir.ipc + "/broker", true) {}

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


class BWatcher : public DirWatcher {
public:
    BWatcher()
    {
    }

    void    dispatch(const std::string& watchedFile, const inotify_event& event, const std::string_view& name) override 
    {
        diag_print(watchedFile, name);
    }
};



void  makeLogFile()
{
    char        buffer[256];
    time_t      n;  
    time(&n);
    strftime(buffer, sizeof(buffer), "%Y%m%d-%H%M%S", localtime(&n));
    buffer[255] = '\0';
    std::string fname   = gDir.log + "/broker-" + buffer + ".log";
    log_to_file(fname, LogPriority::Debug);
}


int execMain(int argc, char* argv[])
{
    //  Configure logging and the directories 
    
        log_to_std_error(LogPriority::Info);
        if(!CommonDir::init()){
            yCritical() << "Cannot create temporary directories under: " << gDir.tmp;
            return -1;
        }
        makeLogFile();
    
    
    //  Check for existing broker, bail if found
    
        PidFile         pidFile(gDir.pid + "/broker");
        if(!pidFile.first()){
            yCritical() << "Existing broker detected, kill it first";
            return 0;
        }
        
    //  Establish sockets

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
        if(dwatch.watch(gDir.ipc, IN_CREATE | IN_DELETE) == -1){
            yError() << "Unable to watch IPC directory!";
        }
        
    //  Register the signal handlers
    
        signal(SIGQUIT, sigQuit);
        signal(SIGTERM, sigQuit);
        signal(SIGHUP, sigRestart);

    //  And ... the main loop, poll between the sockets.
    
        yNotice() << "Starting up the broker!";
        
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
        
    //  Done
    return 0;
}


int main(int argc, char* argv[])
{
    CmdArgs args(argc, argv);
    int r = execMain(argc, argv);
    if((r == 0) && (gQuit == Quit::Restart)){
        yNotice() << "Restart requested.";
        Vector<char*>   args(argv, argv+argc);
        args << (char*) nullptr;
        execv(argv[0], args.data());
    }
    return r;
}

