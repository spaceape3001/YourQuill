////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "FileWatch.hpp"

namespace yq::mithril {
    struct FileWatch::Repo {
        std::vector<const FileWatch*>   all;
    };
    
    FileWatch::Repo& FileWatch::repo()
    {
        static Repo s_repo;
        return s_repo;
    }

    const std::vector<const FileWatch*>& FileWatch::all()
    {
        return repo().all;
    }

    FileWatch::FileWatch(const std::filesystem::path&fp, const std::source_location& sl)
    {
        m_file      = fp;
        m_source    = sl;
        repo().all.push_back(this);
    }
    
    FileWatch::~FileWatch()
    {
    }


    class FunctionalFileWatch : public FileWatch {
    public:
        std::function<void()>   m_function;
        FunctionalFileWatch(std::function<void()> fn, const std::filesystem::path&p, const std::source_location& sl) : 
            FileWatch(p, sl), m_function(fn) 
        {
        }
        
        void handle() const override 
        { 
            m_function(); 
        }
    };

    void on_watch(const std::filesystem::path&fp, std::function<void()>fn, const std::source_location& sl)
    {
        new FunctionalFileWatch(fn, fp, sl);
    }
}

