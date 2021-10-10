#include "CmdArgs.hpp"
#include "StringView.hpp"
#include "ThreadId.hpp"
#include <span>

struct CmdArgs::Repo {
    CmdArgs*                app = nullptr;
    std::span<const char*>  args;
    StringView              exe;
};

CmdArgs::Repo& CmdArgs::repo()
{
    static Repo _repo;
    return _repo;
}


CmdArgs::CmdArgs(int argc, char**argv)
{
    if(!thread_id()){
        Repo& _r    = repo();
        if(!_r.app){
            _r.app      = this;
            _r.args     = std::span<const char*>((const char**) argv, argc);
            _r.exe      = argv[0];
        }
    }
}

CmdArgs::~CmdArgs()
{
    if(!thread_id()){
        Repo& _r    = repo();
        if(_r.app == this)
            _r.app  = nullptr;
    }
}


std::string_view         CmdArgs::appName()
{
    return repo().exe;
}
    
std::span<const char*>   CmdArgs::commandLine()
{
    return repo().args;
}

