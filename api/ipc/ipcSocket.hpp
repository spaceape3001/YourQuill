////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <array>

namespace ipc {

    class Socket {
    public:

        virtual     ~Socket();
        void        read();
        int         fd() const { return m_fd; }

    protected:
        Socket();               // opens a receive socket for this application (should only have ONE per app)
        Socket(const char*);    // opens a receive socket at the specified pathname
        Socket(uint32_t);       // opens a transmit socket to the specified application


    
    private:
        int                         m_fd;
        std::array<uint64_t,65537>  m_buffer;
        
        bool    open_rx(const char*);
        bool    open_tx(const char*);
        
        Socket(const Socket&)                   = delete;
        Socket(Socket&&)                        = delete;
        Socket&     operator=(const Socket&)    = delete;
        Socket&     operator=(Socket&)          = delete;
        auto        operator<=>(const Socket&)  = delete;
    };
}

