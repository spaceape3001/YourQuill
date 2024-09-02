////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once


//#include "WebSession.hpp"
#include <asio/ip/address.hpp>
#include <yq/container/MultiMap.hpp>
#include <mithril/enum/DataRole.hpp>
#include <mithril/enum/SizeDesc.hpp>
#include <yq/basic/Http.hpp>
#include <yq/basic/Url.hpp>
#include <yq/basic/VersionSpec.hpp>
// #include <mithril/text/KV.hpp>
#include <yq/container/ByteArray.hpp>
//#include <mithril/web/HttpHeader.hpp>
#include <yq/typedef/string_maps.hpp>
#include <nlohmann/json.hpp>
#include <time.h>

namespace asio {
    class io_context;
}

namespace yq {
    struct QueryStripped;
}

namespace yq::mithril {
    class WebPage;
    struct RootDir;
    
    struct SessionData {
        std::string_view    ssid;
        std::string         username;
        const RootDir*         def_root        = nullptr;
        unsigned int        columns         = 0;
        SizeDesc            icon_size       = SizeDesc::Small;
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
        
        //std::optional<bool>             auto_edit;
        
        
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
        Vector<char>                    rx_body;

        ContentType                     rx_content_type;

        //! Headers received
        StringViewMultiMap              rx_headers;
        
        //! Json on post (after decoding)
        nlohmann::json                  rx_json;
        
        //! Post Parameter map (after decoding...)
        StringMultiMap                  rx_post;
        
        //! Post Parameter map (no-decoding...)
        StringViewMultiMap              rx_post_raw;
        
        //! Query Parameter map (after decoding...)
        StringMultiMap                  rx_query;

        //! Query Parameter map (no decoding...)
        StringViewMultiMap              rx_query_raw;

        //! Buffers of the received data (don't TOUCH)
        //std::vector<HttpDataPtr>        rx_buffers;

        //! Session Data
        //! \note this data will not persist across connections unless the *set* method is called
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

        //! Used for redirects....
        Url                             tx_redirect;

        //! URL requested
        UrlView                         url;
        
        //! Path beyond what resolved (ie, for glob patterns)
        std::string_view                truncated_path;

        //! Body variable
        std::string                     var_body;
        
        //! Extra (context-dependent) scripts
        std::vector<std::string>        var_scripts;
        
        //! Extra javascript
        std::string                     var_script;

        //! Title variable
        std::string                     var_title;

        //! Version of the request
        VersionSpec                     version;
        
        bool                            can_edit() const;
        
        
        unsigned int                    columns() const;

        //! Decodes body to post parameters
        void                            decode_post();
        
        //! Decodes the query
        void                            decode_query();
        
        //! Decodes body to json
        void                            decode_json();
        
        const RootDir*                     def_root(DataRole) const;
        
        bool                            edit_now();
        
        //! Decodes the first query parameter found by the given name (ignoring the rest)
        std::string                     find_query(std::string_view) const;
        std::string                     find_query(std::initializer_list<std::string_view>) const;
        

        
        //void                            tx_reset(bool resetStatus=false);
        //void                            simple_content(std::string_view);
        
        bool                            is_local() const { return static_cast<bool>(flags & LOCAL); }
        
        bool                            is_mobile() const;
        
        void                            redirect(std::string_view where, bool permanent=false);

        void                            return_to_sender(HttpStatus hWhy=HttpStatus::SeeOther);
        
        std::string_view                rx_header(std::string_view) const;

        QueryStripped                   strip_query(std::string_view k, bool first=false) const;
        

    //  virtuals.....

        virtual void                    tx_header(std::string_view k, std::string_view v) = 0;

        virtual void                    set_admin(bool) = 0;
        virtual void                    set_auto_edit(bool) = 0;
        virtual void                    set_columns(unsigned int) = 0;
        virtual void                    set_def_root(const RootDir*) = 0;
        virtual void                    set_inspect_submit(bool) = 0;
        virtual void                    set_icon_size(SizeDesc) = 0;
        virtual void                    set_logged_in(bool) = 0;
        virtual void                    set_username(std::string_view) = 0;
        
        WebContext(asio::io_context& _io_ctx);
        ~WebContext();
    };

}
