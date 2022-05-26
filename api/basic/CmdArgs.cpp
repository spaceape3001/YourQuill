////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "CmdArgs.hpp"
#include "ThreadId.hpp"
#include <span>
#include <string_view>

namespace yq {
    struct CmdArgs::Repo {
        CmdArgs*                app = nullptr;
        std::span<const char*>  args;
        std::string_view        exe;
    };

    CmdArgs::Repo& CmdArgs::repo()
    {
        static Repo _repo;
        return _repo;
    }


    CmdArgs::CmdArgs(int argc, char**argv)
    {
        if(!thread::id()){
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
        if(!thread::id()){
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
}
