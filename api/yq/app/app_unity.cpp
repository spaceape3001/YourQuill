#include "CmdArgs.hpp"
#include "DelayInit.hpp"
#include "PidFile.hpp"

#include <yq/file/FileUtils.hpp>
#include <yq/log/Logging.hpp>
#include <yq/text/Utils.hpp>

#include <atomic>
#include <span>
#include <fstream>

#include <dlfcn.h>

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

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  PID FILE
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    bool  load_plugin(const std::filesystem::path&pth)
    {
        if(thread::id()){
            yCritical() << "Plugins should only be loaded on the main thread!";
            return false;
        }
    
        #ifdef NDEBUG
        try {
        #endif
            if(!dlopen(pth.c_str(), YQ_DBGREL(RTLD_LAZY|RTLD_LOCAL, RTLD_NOW|RTLD_LOCAL)))
                yError() << "Plugin (" << pth << ") failed to load.";
            return true;
            
        #ifdef NDEBUG
        } catch(...){
            return false;
        }
        #endif
    }
    
    size_t  load_plugin_dir(const std::filesystem::path&pdir)
    {
        if(thread::id()){
            yCritical() << "Plugins should only be loaded on the main thread!";
            return 0;
        }
        
        if(!std::filesystem::is_directory(pdir)){
            yWarning() << "Not a directory " << pdir;
        }

        //  Update this for the operating system
        #if defined(WIN32)
        static constexpr const char *szExt   = ".dll";
        #else
        static constexpr const char *szExt   = ".so";
        #endif

        size_t      cnt   = 0;
        
        dir::for_all_children(pdir, dir::NO_DIRS, [&](const std::filesystem::path&p){
            if(!is_similar(p.extension().c_str(), szExt))
                return ;
            if(load_plugin(p))
                ++cnt;
        });
        return cnt;
    }
    
}
