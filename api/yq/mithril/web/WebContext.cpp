////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "WebContext.hpp"
#include <mithril/wksp/Workspace.hpp>
#include <string_view>
#include <yq/core/Result.hpp>
#include <yq/text/basic.hpp>
#include <yq/text/copy.hpp>
#include <yq/text/misc.hpp>
#include <yq/text/match.hpp>
#include <yq/text/parse.hpp>
#include <yq/text/vsplit.hpp>
#include <yq/net/HttpParser.hpp>

namespace yq::mithril {
    WebContext::WebContext(asio::io_context& _io_ctx) :io_ctx(_io_ctx)
    {
    }
    
    WebContext::~WebContext()
    {
    }

    bool                        WebContext::can_edit() const
    {
        return true;
        //  TODO
        //return is_local();
    }

    unsigned int                WebContext::columns() const
    {
        unsigned int c  = to_uinteger(find_query({"cols", "columns"})).value_or(0);
        if(c)
            return c;
        if(session.columns)
            return session.columns;
        return is_mobile() ? 1 : 6;
    }
    
    
    void                      WebContext::decode_json() 
    {
        if((rx_content_type != ContentType()) && (rx_content_type != ContentType::json))
            return ;
        if(!rx_json && !rx_body.empty())
            rx_json = nlohmann::json::parse(rx_body.begin(), rx_body.end());
    }


    //! Decodes post parameters
    void                        WebContext::decode_post() 
    {
        if((rx_content_type != ContentType()) && (rx_content_type != ContentType::form))
            return ;

        if(rx_post.empty() && !rx_body.empty()){
            rx_post_raw = parse_parameters(std::string_view(rx_body.data(), rx_body.size()));
            rx_post     = web_decode(rx_post_raw);
        }
    }

    void                WebContext::decode_query() 
    {
        if(rx_query.empty() && !url.query.empty()){
            rx_query_raw    = parse_parameters(url.query);
            rx_query        = web_decode(rx_query_raw);
        }
    }

    const RootDir*         WebContext::def_root(DataRole dr) const
    {
        if(session.def_root)
            return session.def_root;
        return wksp::root_reads(dr).value(0, nullptr);
    }
    
    bool                WebContext::edit_now()
    {
        decode_post();

        std::string     estr    = rx_post.first("edit");
        if(estr.empty())
            return session.auto_edit;
        
        if(is_similar(estr, "later")){
            set_auto_edit(false);
            return false;
        }
        
        if(is_similar(estr, "now")){
            set_auto_edit(true);
            return true;
        }
        
        return session.auto_edit;
    }

    std::string         WebContext::find_query(std::string_view k) const
    {
        // bypass if queried already
        if(!rx_query.empty())
            return rx_query.first(copy(k));
    
        return vsplit(url.query, '&', [&](std::string_view b) -> std::string {
            const char* eq  = strnchr(b, '=');
            if(!eq)
                return std::string();
            if(!is_similar(k, std::string_view(b.data(), eq)))
                return std::string();
            return web_decode(std::string_view(eq+1, b.end()));
        });
    }


    std::string         WebContext::find_query(std::initializer_list<std::string_view> keys) const
    {
        // bypass if queried already
        if(!rx_query.empty())
            return rx_query.first(copy(keys));

        return vsplit(url.query, '&', [&](std::string_view b) -> std::string {
            const char* eq  = strnchr(b, '=');
            if(!eq)
                return std::string();
            if(!is_in(std::string_view(b.data(), eq), keys))
                return std::string();
            return web_decode(std::string_view(eq+1, b.end()));
        });
    }

    bool    WebContext::is_mobile() const 
    { 
        // TODO
        return false; 
    }

    void    WebContext::redirect(std::string_view sv, bool permanent)
    {
        if(status != HttpStatus()){
            if(!isRedirect(status)){
                status    = HttpStatus::InternalError;
                return ;
            }
        } else {
            status = permanent ? HttpStatus::MovedPermanently : HttpStatus::TemporaryRedirect;
        }
        
        tx_header("Location", sv);
    }

    void                WebContext::return_to_sender(HttpStatus hWhy)
    {
        status  = hWhy;
        tx_redirect         = copy(to_url_view(rx_header("referer")).value);
    }
    
    std::string_view    WebContext::rx_header(std::string_view k) const
    {
        return rx_headers.first(k);
    }

    QueryStripped       WebContext::strip_query(std::string_view k, bool first) const
    {
        return stripped_query(k, url.query, first);
    }
}
