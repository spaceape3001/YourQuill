////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <string_view>

struct inotify_event;

namespace ipc {

    class DirWatcher {
    public:
        DirWatcher();
        ~DirWatcher();
        
        int         fd() const { return m_fd; }
        
        // returns FALSE if fatal error occurs
        bool        process();
        
    protected:
        //virtual void    
        
    private:

        DirWatcher(const DirWatcher&) = delete;
        DirWatcher(DirWatcher&&) = delete;
        DirWatcher& operator=(const DirWatcher&) = delete;
        DirWatcher& operator=(DirWatcher&&) = delete;
        bool operator<=>(const DirWatcher&) = delete;

        void        dispatch(const inotify_event&, const std::string_view&);

        int     m_fd;
    };
}
