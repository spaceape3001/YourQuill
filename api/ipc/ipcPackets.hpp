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
    
    
    App IDs ... Running instance ID for an application.
    
    Workspace 0x0000 is NO workspace,
    Workspace 0xFFFF is for ALL workspaces
    
    App ID 0x0000 is wildcard for either the broker (wksp #0) or the updator
    App ID 0xFFFF is wildcard for ALL apps
    
    So, wksp=FFFF, tgt=0000, broadcasts to all updaters
    And
        wksp=FFFF, tgt=FFFF, brodcasts to EVERYBODY
    And
        wksp=(id), tgt=FFFF, brodcasts to everybody using that particular workspace
*/

namespace ipc {
    //  special PIDs
    static constexpr const uint32_t     ME              = 0x00000000;
    static constexpr const uint32_t     YOU             = 0xFFFFFFFF;


    /*! \brief IPC Header
        
        This is the header that precedees EVERY packet.
    */
    struct Packet {
        uint32_t        origin;     // originating PID
        uint16_t        opcode;     // the OPCODE
        uint16_t        payload;    // payload size, number of QWORDS (including structures), so limit is 512k
    };


    /*! \brief Startup  Infomation
        Nice thing, everything's implied by the origin PID... :)
    */
    struct Starting {
        static constexpr const uint16_t     OPCODE      = 0x0011;
        uint64_t        token;      // OUR application token, needed for subsequenet (powerful) commands
    };

    /*! \brief Application is stopping (should be final)
    */
    struct Stopping {
        static constexpr const uint16_t     OPCODE      = 0x0012;
    };


    /*! \brief Request for a shutdown, sent to the application
    */
    struct ShutdownCmd {
        static constexpr const uint16_t     OPCODE      = 0x0013;
    };

        /*! \brief Request to RESTART a particular application
        
        */
    struct RestartReq {
        static constexpr const uint16_t     OPCODE      = 0x0014;
        
        //! \brief App to restart.  (YOU is invalid)
        uint32_t    app;
        
        //! \brief Delay to  (in us)
        uint32_t    delay;
    };
    
    
    struct StartingInfo  {
        static constexpr const uint16_t     OPCODE      = 0x001C;
        //!     Server PID
        uint32_t        server;
        //!     Updater PID
        uint32_t        updater;
        //!     Workspace ID
        uint32_t        wksp;
        //!     Number of other running apps in workspace
        uint32_t        apps;
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
