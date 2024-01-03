////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ipcBuffer.hpp"
#include "ipcSocket.hpp"
#include <0/basic/Logging.hpp>

namespace yq::mithril::ipc {

    Socket::Socket() : m_packet{}
    {
    }

    Socket::Socket(const std::filesystem::path& z, bool isRead) : Socket()
    {
        if(isRead)
            open_rx(z);
        else
            open_tx(z);
    }
    

    Socket::~Socket()
    {
        close();
    }

    void    Socket::close()
    {
        if(m_fd != -1){
            ::close(m_fd);
            m_fd        = -1;
        }
    }

    bool        Socket::open(const std::filesystem::path& z, unsigned int mode)
    {
        if(m_fd != -1){
            yError() << "Socket is already open!";
            return false;
        }
        
        if(mkfifo(z.c_str(), 0666) && (errno != EEXIST)){
            yError() << "Unable to make FIFO, aborting.";
            return false;
        }
        
        m_fd        = ::open(z.c_str(), mode);
        if(m_fd != -1){
            m_path  = z;
            return true;
        } else
            return false;
    }

    //bool        Socket::open_rx()
    //{
        //return false; // open_rx(gDir.ipc / String::number(getpid()).c_str());
    //}

    bool        Socket::open_rx(const std::filesystem::path& z)
    {
        m_reading   = true;
        if(!open(z, O_RDONLY|O_NONBLOCK))
            return false;
            
        fcntl(m_fd, F_SETPIPE_SZ, (int) (1 << 20)); // increasse the buffer to a megabyte (sys-limit)
        return true;
    }


    bool        Socket::open_tx(const std::filesystem::path&s)
    {   
        m_reading   = false;
        return open(s, O_WRONLY);
    }

    bool        Socket::process()
    {
        static constexpr const size_t   N   = 1 << 16;
        
        if(m_fd == -1){
            yCritical() << "Cannot process a closed IPC socket!";
            return false;
        }
        
        Buffer<N>           buffer;

        const char*         p       = nullptr;
        const char*         t       = nullptr;
        const char*         u       = nullptr;
        const Starting*     rstart  = nullptr;
        const StartInfo*    rsti    = nullptr;
        const RestartReq*   rrstq   = nullptr;
        const Packet*       evt = nullptr;
        
        for(;;){
            switch(buffer.read_from(m_fd)){
            case ReadResult::Error:
                {
                    int y   = errno;
                    yError() << "Error in reading socket: " << strerror(y);
                    return false;
                }
                break;
            case ReadResult::Empty:
                return true;
            case ReadResult::Read:
                break;
            }
            

            for(p = buffer.begin(); p<buffer.end(); ){
                evt         = (const Packet*) p;
                m_packet    = *evt;
                p          += sizeof(Packet);
                t           = p;
                p          += evt->len;
                switch(evt->opcode){
                case Print::OPCODE:
                    rxPrint(std::string_view(t, evt->len));
                    break;
                case PrintWkspListReq::OPCODE:
                    rxPrintWkspListing();
                    break;
                case Restart::OPCODE:
                    rxRestart();
                    break;
                case RestartReq::OPCODE:
                    rrstq   = (const RestartReq*) t;
                    rxRestartReq(rrstq->app, rrstq->delay);
                    break;
                case Shutdown::OPCODE:
                    rxShutdown();
                    break;
                case Starting::OPCODE:
                    rstart      = (const Starting*) t;
                    u           = t + sizeof(Starting);
                    rxStarting(rstart->token, rstart->pid, std::string_view(u, evt->len - sizeof(Starting)));
                    break;
                case StartInfo::OPCODE: 
                    rsti        = (const StartInfo*) t;
                    m_origin    = rsti -> yourId;
                    break;
                default:
                    //  skip-it
                    break;
                }
            }
        }
    }

    template <typename P>
    void        Socket::tx(uint8_t op, P pred)
    {
        if(m_fd == -1)
            return;
        
        W       w;
        Packet* pkt = w.add<Packet>();
        pkt -> origin   = m_origin;
        pkt -> opcode   = op;
        pred(w);
        pkt -> len      = (uint8_t)(w.size() - sizeof(Packet));
        w.write_to(m_fd);
    }

    void        Socket::tx(uint8_t op)
    {
        tx(op, [](W&){});
    }

    void        Socket::txPrint(const std::string_view& k)
    {
        tx(Print::OPCODE, [&](W& w){
            w << k;
        });
    }

    void        Socket::txPrintWkspListing()
    {
        tx(PrintWkspListReq::OPCODE);
    }

    void        Socket::txRestart()
    {
        tx(Restart::OPCODE);
    }

    void        Socket::txRestartReq(uint32_t app, uint32_t delay)
    {
        tx(Restart::OPCODE, [&](W&w){
            auto* p = w.add<RestartReq>();
            p->app       = app;
            p->delay     = delay;
        });
    }

    void        Socket::txStarting(uint64_t token, const std::string_view& wk)
    {
        tx(Starting::OPCODE, [&](W&w){
            auto* p = w.add<Starting>();
            p->token    = token;
            p->pid      = getpid();
            w << wk;
        });
    }

    void        Socket::txStopping()
    {
        tx(Stopping::OPCODE);
    }

    void        Socket::txShutdown()
    {
        tx(Shutdown::OPCODE);
    }
}
