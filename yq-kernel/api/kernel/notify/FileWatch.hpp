////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <filesystem>
#include <functional>
#include <source_location>
#include <vector>

namespace yq {

    // simplified version of the notifier, keyed to exact files instead
    class FileWatch {
    public:
    
        static const std::vector<const FileWatch*>& all();
        
        virtual void    handle() const = 0;
        
        const std::filesystem::path&        file() const { return m_file; }
        const std::source_location&         source() const { return m_source; }
    
    protected:
        FileWatch(const std::filesystem::path&, const std::source_location& );
        ~FileWatch();
    private:
        std::filesystem::path       m_file;
        std::source_location        m_source;
        
        struct Repo;
        static Repo& repo();
    };
    
    template <void (*FN)()>
    class VoidFileWatch : public FileWatch {
    public:
        VoidFileWatch(const std::filesystem::path&p, const std::source_location& sl) : FileWatch(p, sl) {}
        void handle() const override 
        { 
            FN(); 
        }
    };
    
    template <void (*FN)()>
    void on_change(const std::filesystem::path&fp, const std::source_location& sl=std::source_location::current())
    {
        new VoidFileWatch<FN>(fp, sl);
    }
    
    void on_change(const std::filesystem::path&, std::function<void()>, const std::source_location& sl=std::source_location::current());


    template <void (*FN)(const std::filesystem::path&)>
    class PathFileWatch : public FileWatch {
    public:
        PathFileWatch(const std::filesystem::path&p, const std::source_location& sl) : FileWatch(p, sl) {}
        void handle() const override 
        { 
            FN(file()); 
        }
    };
    
    template <void (*FN)(const std::filesystem::path&)>
    void on_change(const std::filesystem::path&fp, const std::source_location& sl)
    {
        new PathFileWatch<FN>(fp, sl);
    }
}

