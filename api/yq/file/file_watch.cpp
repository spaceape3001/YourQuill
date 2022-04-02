////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "DirWatcher.hpp"
#include "Notifier.hpp"
#include <yq/ipc/ipcBuffer.hpp>
#include <yq/log/Logging.hpp>
#include <yq/text/Utils.hpp>
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
    
    struct Notifier::Repo {
        std::vector<const Notifier*>    all;
        EnumMap<Change,Vector<const Notifier*>> byChange;
    };
    
    Notifier::Repo&  Notifier::repo()
    {
        static Repo s_repo;
        return s_repo;
    }

    const std::vector<const Notifier*>&  Notifier::all()
    {
        return repo().all;
    }
    
    const EnumMap<Change,Vector<const Notifier*>>&     Notifier::change_map()
    {
        return repo().byChange;
    }
    

    Notifier::Notifier(Trigger trigger, Flag<Change> changeMask, Folder folder, std::string_view ext, const std::filesystem::path& file, const std::source_location& sl)
    {
        m_path      = file;
        m_source    = sl;
        m_folder    = folder;
        m_change    = changeMask;

        bool        is_extension    = starts(ext, "*.");
        if(is_extension)
            ext     = ext.substr(2);
        m_specific  = ext;
        
        switch(trigger){
        case NoTrigger:
        case ByFile:
            break;
        case ByDocument:
        case ByExtension:
        case ByFolderExt:
        case ByFolderDoc:
            m_trigger   = trigger;
            break;
        case SpecName:
            m_trigger   = is_extension ? ByExtension : ByDocument;
            break;
        case SpecFolderName:
            m_trigger   = is_extension ? ByFolderExt : ByFolderDoc;
            break;
        default:
            m_trigger   = NoTrigger;
            break;
        }

        Repo&   _r = repo();
        _r.all.push_back(this);
        for(Change c : Change::all_values())
            if(changeMask.is_set(c))
                _r.byChange[c] << this;
    }
    
    Notifier::~Notifier()
    {
    }

    Notifier::Writer&     Notifier::Writer::description(std::string_view d)
    {
        if(importer)
            importer -> m_description = d;
        return *this;
    }
}
