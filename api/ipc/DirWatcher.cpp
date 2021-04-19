////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "DirWatcher.hpp"
#include "ipcBuffer.hpp"
#include <util/Logging.hpp>

#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/inotify.h>

namespace ipc {
    DirWatcher::DirWatcher()
    {
        m_fd        = inotify_init1(IN_NONBLOCK);
    }

    DirWatcher::~DirWatcher()
    {
    }


    void        DirWatcher::dispatch(const inotify_event&, const std::string_view&)
    {
        //  TODO.....
    }

    bool        DirWatcher::process()
    {
        if(m_fd == -1){
            yCritical() << "Cannot process a closed IPC dir watcher!";
            return false;
        }
    
        static constexpr const size_t   N   = 1 << 16;
        Buffer<N>         buffer;
        const inotify_event*    evt = nullptr;
        for(;;){
            switch(buffer.read_from(m_fd)){
            case ReadResult::Error:
                {
                    int y   = errno;
                    yError() << "DirWatcher failure: " << strerror(y);
                    return false;
                }
                break;
            case ReadResult::Empty:
                return true;
            case ReadResult::Read:
                break;
            }

            for(char* p = buffer.begin(); p<buffer.end(); p += sizeof(inotify_event)+evt->len){
                evt   = (const inotify_event*) p;
                dispatch(*evt, std::string_view(evt->name, evt->len));
            }
        }
        return false;
    }

}
