#include "CmdArgs.hpp"
#include "DelayInit.hpp"
#include "PidFile.hpp"

#include <util/file/FileUtils.hpp>

#include <atomic>
#include <span>
#include <fstream>

namespace yq {

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  COMMAND ARGS
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  DELAY INIT
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////



    DelayInit::DelayInit() : m_next(nullptr)
    {
        thread::id();    // simply to force the thread ID...
        m_next  = current(this);
    }

    void         DelayInit::init_all(bool fRepeat)
    {
        while(init_pass() && fRepeat)
            ;
    }

    bool        DelayInit::init_pass()
    {
        DelayInit*     top = current(nullptr);
        if(!top)
            return false;
        
        DelayInit*              x   = nullptr;
        DelayInit*              n   = nullptr;
        
        for(x=top;x;x=n){
            n   = x -> m_next;
            x -> m_next = nullptr;
            x -> initialize();
        }

        return true;
    }



    DelayInit*  DelayInit::current(DelayInit* n)
    {
        static std::atomic<DelayInit*>   ptr(nullptr);
        return ptr.exchange(n);
    }

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  PID FILE
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    PidFile::PidFile(const std::filesystem::path& f, bool autoMake)
    {
        m_file  = f;
        m_first = !file_exists(m_file.c_str());
        if(m_first && autoMake)
            make();
        
    }

    void     PidFile::make()
    {
        std::ofstream   p(m_file);
        p << getpid();
    }

    PidFile::~PidFile()
    {
        if(m_first){
            remove(m_file.c_str());
        }
    }
}
