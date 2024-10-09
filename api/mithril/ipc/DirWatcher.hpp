////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <string_view>
#include <yq-toolbox/container/Map.hpp>
#include <yq/trait/not_copyable.hpp>
#include <yq/trait/not_moveable.hpp>
#include <sys/inotify.h>


namespace yq::mithril {

    class DirWatcher : not_copyable, not_moveable {
    public:
        DirWatcher();
        ~DirWatcher();
        
        int         fd() const { return m_fd; }
        
        // returns FALSE if fatal error occurs
        bool        process();
        
        int         watch(const std::string&, uint32_t mask);
        int         descriptor(const std::string&);
        
        
    protected:

        //  called when an event happened
        virtual void    dispatch(std::string_view, const inotify_event& event, std::string_view name);
        void            diag_print(std::string_view watchedFile, std::string_view name);
        
    private:

        bool operator<=>(const DirWatcher&) = delete;

        struct Watch {
            std::string     path;
            uint32_t        mask    = 0;
        };

        int                     m_fd;
        Map<int,Watch>          m_id2wd;
        Map<std::string,int>    m_name2id;
    };

}
