////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "DirWatcher.hpp"
#include "ipcBuffer.hpp"
#include "ipcPackets.hpp"
#include "ipcSocket.hpp"
#include <yq/log/Logging.hpp>

namespace yq {

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  DIRECTORY WATCHING
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

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
    //  IPC SOCKETS
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


    namespace ipc {

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


    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
}


#if 0
#include "ipcBuffer.hpp"
#include "ipcSocket.hpp"
#include "PidFile.hpp"

#include <yq/CommonDirs.hpp>

#include <yq/FileUtils.hpp>
#include <yq/Logging.hpp>
#include <yq/String.hpp>
#include <yq/Vector.hpp>

#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <fstream>

#include "DirWatcher.ipp"
#include "PidFile.ipp"
#include "ipcSocket.ipp"
#endif
