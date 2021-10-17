////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "ipcPackets.hpp"
#include <filesystem>
#include <string>

namespace ipc {
    using fspath =  std::filesystem::path;
    template <size_t> class Buffer;

    class Socket {
    public:

        virtual         ~Socket();
        bool            process();
        int             fd() const { return m_fd; }

            // uses current process ID in IPC dir
        bool            open_rx();
        bool            open_rx(const fspath&);
        bool            open_tx(const fspath&);

        void            close();

        void            txPrint(const std::string_view&);
        void            txPrintWkspListing();
        void            txRestart();
        void            txRestartReq(uint32_t app, uint32_t delay);
        void            txStarting(uint64_t token, const std::string_view& wksp);
        void            txStartingInfo(uint32_t serverPid, uint32_t updaterPid, uint32_t wkspId);
        void            txStopping();
        void            txShutdown();

        const fspath&   path() const { return m_path; }

        Socket();      
        Socket(const fspath&, bool isRead);


    protected:
        bool            open(const fspath&, unsigned int mode);
        uint16_t        origin() const { return m_packet.origin; }
        uint8_t         opcode() const { return m_packet.opcode; }
        
    
        virtual void    rxPrint(const std::string_view&) {}
        virtual void    rxPrintWkspListing(){}
        virtual void    rxRestart() {}
        virtual void    rxRestartReq(uint32_t app, uint32_t delay) {}
        virtual void    rxStarting(uint64_t token, uint32_t pid, const std::string_view&) {}
        virtual void    rxStartingInfo(uint32_t serverPid, uint32_t updaterPid, uint32_t wkspId) {}
        virtual void    rxStopping(){}
        virtual void    rxShutdown(){}
    
    
    private:
        int             m_fd        = -1;
        uint16_t        m_origin    = 0;
        bool            m_reading   = false;
        Packet          m_packet;
        fspath          m_path;
        
        using W         = Buffer<256>;
        
        template <typename P>
        void            tx(uint8_t, P);
        
        void            tx(uint8_t);
        
        Socket(const Socket&)                   = delete;
        Socket(Socket&&)                        = delete;
        Socket&     operator=(const Socket&)    = delete;
        Socket&     operator=(Socket&)          = delete;
        auto        operator<=>(const Socket&)  = delete;
    };
}

