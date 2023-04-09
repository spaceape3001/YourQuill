////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "DirWatcher.hpp"
#include <aether/http/ipc/ipcBuffer.hpp>
#include <basic/Logging.hpp>
#include <basic/TextUtils.hpp>
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

    void    DirWatcher::dispatch(std::string_view, const inotify_event&, std::string_view) 
    {
    }

    void        DirWatcher::diag_print(std::string_view watchedFile, std::string_view name)
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


}
