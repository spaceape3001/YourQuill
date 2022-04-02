////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "DirWatcher.hpp"
#include "Importer.hpp"
#include <yq/ipc/ipcBuffer.hpp>
#include <yq/log/Logging.hpp>
#include <vector>

namespace yq {

    DirWatcher::DirWatcher()
    {
        m_fd        = inotify_init1(IN_NONBLOCK);
    }

    DirWatcher::~DirWatcher()
    {
        m_name2id.clear();
        m_id2wd.clear();
    }

    int         DirWatcher::descriptor(const std::string&k)
    {
        auto i = m_name2id.find(k);
        if(i != m_name2id.end())
            return i->second;
        return -1;
    }

    void        DirWatcher::diag_print(const std::string& watchedFile, const std::string_view& name)
    {
        yInfo() << "File change detected: " << watchedFile << "/" << name;
    }

    bool        DirWatcher::process()
    {
        if(m_fd == -1){
            yCritical() << "Cannot process a closed IPC dir watcher!";
            return false;
        }

        static constexpr const size_t   N   = 1 << 16;
        ipc::Buffer<N>          buffer;
        const inotify_event*    evt = nullptr;
        for(;;){
            switch(buffer.read_from(m_fd)){
            case ipc::ReadResult::Error:
                {
                    int y   = errno;
                    yError() << "DirWatcher failure: " << strerror(y);
                    return false;
                }
                break;
            case ipc::ReadResult::Empty:
                return true;
            case ipc::ReadResult::Read:
                break;
            }

            for(char* p = buffer.begin(); p<buffer.end(); p += sizeof(inotify_event)+evt->len){
                evt   = (const inotify_event*) p;
                auto j = m_id2wd.find(evt->wd);
                if(j != m_id2wd.end())
                    dispatch(j->second.path, *evt, std::string_view(evt->name, evt->len));
            }
        }
        return false;
    }

    int         DirWatcher::watch(const std::string& k, uint32_t mask)
    {
        auto i  = m_name2id.find(k);
        if(i != m_name2id.end())
            return i->second;
            
        int j   = inotify_add_watch(m_fd, k.c_str(), mask);
        if(j == -1)
            return -1;
            
        Watch w;
        w.path  = k;
        w.mask  = mask;
        m_name2id[k]    = j;
        m_id2wd[j]      = w;
        return j;
    }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    
    struct Importer::Repo {
        std::vector<const Importer*>    all;
    };
    
    Importer::Repo&  Importer::repo()
    {
        static Repo s_repo;
        return s_repo;
    }

    const std::vector<const Importer*>&  Importer::all()
    {
        return repo().all;
    }
    
    Importer::Importer(Trigger trigger, Flag<Change> changeMask, Folder folder, std::string_view ext, const std::filesystem::path& file, const std::source_location& sl)
    {
        m_extension = ext;
        m_path      = file;
        m_source    = sl;
        m_folder    = folder;
        m_trigger   = trigger;
        m_change    = changeMask;
        repo().all.push_back(this);
    }
    
    Importer::~Importer()
    {
    }

    Importer::Writer&     Importer::Writer::description(std::string_view d)
    {
        if(importer)
            importer -> m_description = d;
        return *this;
    }
}
