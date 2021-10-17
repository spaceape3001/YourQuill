////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <stdint.h>
#include <string_view>

/*
    IPC here refers to the interprocessor communication protocol, uses to communicate between the various applications.
    
    Workspace IDs   .... likely overprovisioned, is the identifier for a specific workspace.
    
    PIDs are used for App IDs.   Special PIDs are, 0 (origin app) or 0xFFFFFFFF (target app)
    
    MAX_PACKET is 1024, and one string PER packet
    
*/

namespace ipc {
    //  special PIDs
    static constexpr const uint32_t     ME              = 0x00000000;
    static constexpr const uint32_t     YOU             = 0xFFFFFFFF;

    /*! \brief IPC Header
        
        This is the header that precedees EVERY packet.
    */
    struct Packet {
        uint16_t        origin;
        uint8_t         opcode;     // the OPCODE
        uint8_t         len;        // payload size
    };


    struct Print {
        static constexpr const uint16_t     OPCODE         = 0;
    };


    /*! \brief Startup  Infomation
        Nice thing, everything's implied by the origin PID... :)
    */
    struct Starting {
        static constexpr const uint8_t      OPCODE      = 1;
        uint64_t        token;      // OUR application token, needed for subsequenet (powerful) commands
        uint32_t        pid;
        
        //  "socket back"  after this
    };
    
    struct StartInfo {
        static constexpr const uint8_t      OPCODE      = 2;
        uint16_t        yourId;
        //uint32_t        server;
        //uint32_t        updater;
        //uint16_t        wkspId;
    };
    

    /*! \brief Application is stopping (should be final)
    */
    struct Stopping {
        static constexpr const uint8_t      OPCODE      = 3;
    };


    /*! \brief Request for a shutdown, sent to the application
    */
    struct Shutdown {
        static constexpr const uint8_t      OPCODE      = 4;
    };
    
    struct Restart {
        static constexpr const uint8_t      OPCODE      = 5;
    };

        /*! \brief Request to RESTART a particular application
        
        */
    struct RestartReq {
        static constexpr const uint8_t      OPCODE       = 6;
        
        //! \brief App to restart.  (YOU is invalid)
        uint32_t    app;
        
        //! \brief Delay to  (in us)
        uint32_t    delay;
    };
    
    struct PrintWkspListReq {
        static constexpr const uint8_t      OPCODE      = 7;
    };
    
    
    //struct WkspLookupReq {
        //static constexpr const uint16_t     OPCODE      = 0x1001;
    //};
    
    //struct WkspAppReply {
        //static constexpr const uint16_t     OPCODE      = 0x0101;
        //uint32_t        server, updater;
    //};
    
    
    //struct WkspInfoReply {
        //uint32_t        id;
        //uint32_t        server, updater;
        
    //};
    
    

}
