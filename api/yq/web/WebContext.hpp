////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once


//#include "WebSession.hpp"
#include <asio/ip/address.hpp>
#include <yq/collection/MultiMap.hpp>
#include <yq/enum/SizeDesc.hpp>
#include <yq/net/Http.hpp>
#include <yq/net/Url.hpp>
#include <yq/net/VersionSpec.hpp>
// #include <yq/text/KV.hpp>
#include <yq/type/ByteArray.hpp>
//#include <yq/web/HttpHeader.hpp>
#include <nlohmann/json_fwd.hpp>
#include <time.h>

namespace asio {
    class io_context;
}

namespace yq {
    class WebPage;
    struct Root;
    
    struct SessionData {
        std::string_view    ssid;
        std::string         username;
        const Root*         def_root        = nullptr;
        unsigned int        columns         = 0;
        SizeDesc            icon_size;
        bool                auto_edit       = false;
        bool                can_edit        = false;
        bool                logged_in       = false;
        bool                inspect_submit  = false;
        bool                admin           = false;
    };
    
    
    struct WebContext  {
        enum : uint64_t {
            LOCAL           = 1ULL << 0
        };
        
        //std::string_view                cookie_magic;
        
        //! Informational Flags about this... 
        uint32_t                        flags               = 0;

        //! Host of the request
        std::string_view                host;
        
        //! ASIO IO context governing this
        asio::io_context&               io_ctx;

        //! Magic value from session
        std::string                     magic;

        //MarkdownContext                 markdown;

        //! Method of the request
        HttpOp                          method;
        
        //! Our webpage....
        const WebPage*                  page            = nullptr;
        
        //! Should be our port, if any is specified
        uint16_t                        port            = 0;

        //! Port they originated from
        uint16_t                        remote_port     = 0;

        //! Address of the remote user
        asio::ip::address               remote_addr;

        //! Resolved filename (for extension handlers)
        std::filesystem::path           resolved_file;
        
        //! The received content
        std::vector<char>               rx_body;

        ContentType                     rx_content_type;

        //! Headers received
        StringViewMultiMap              rx_headers;
        
        //! Buffers of the received data (don't TOUCH)
        //std::vector<HttpDataPtr>        rx_buffers;

        //! Session Data
        //! \note this data will not persist across sessions unless the *set* method is called
        SessionData                     session;

        //! Status to reply with
        HttpStatus                      status;
        

        //! Time of dispatch
        time_t                          time;
        
        struct tm                       timeparts;
        
        //! Time text
        char                            timestamp[64];

        //! Content to transmit
        std::shared_ptr<ByteArray>      tx_content;

        //! Type of the data being returned
        ContentType                     tx_content_type;

        //! URL requested
        UrlView                         url;
        
        //! Path beyond what resolved (ie, for glob patterns)
        std::string_view                truncated_path;

        //! Body variable
        std::string                     var_body;
        
        //! Extra (context-dependent) scripts
        std::vector<std::string>        var_scripts;

        //! Title variable
        std::string                     var_title;

        //! Version of the request
        VersionSpec                     version;
        

        //! Decodes post parameters
        StringViewMultiMap              decode_post() const;
        
        //! Decodes ALL queries
        StringViewMultiMap              decode_query() const;
        
        //! Converts the "body" to json
        nlohmann::json                  decode_json() const;
        
        
        //! Decodes the first query parameter found by the given name (ignoring the rest)
        std::string                     find_query(std::string_view) const;
        std::string                     find_query(std::initializer_list<std::string_view>) const;
        
        virtual void                    tx_header(std::string_view k, std::string_view v) = 0;

        void                            redirect(std::string_view where, bool permanent=false);
        
        //void                            tx_reset(bool resetStatus=false);
        //void                            simple_content(std::string_view);
        
        bool                            is_local() const { return static_cast<bool>(flags & LOCAL); }
        
        virtual void                    set_admin(bool) = 0;
        virtual void                    set_auto_edit(bool) = 0;
        virtual void                    set_columns(int) = 0;
        virtual void                    set_def_root(const Root*) = 0;
        virtual void                    set_inspect_submit(bool) = 0;
        virtual void                    set_icon_size(SizeDesc) = 0;
        virtual void                    set_logged_in(bool) = 0;
        virtual void                    set_username(std::string_view) = 0;
        
        WebContext(asio::io_context& _io_ctx);
        ~WebContext();
    };
}
