////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/wksp/Workspace.hpp>

namespace yq {
    WebContext::WebContext(asio::io_context& _io_ctx) :io_ctx(_io_ctx)
    {
    }
    
    WebContext::~WebContext()
    {
    }
    
    nlohmann::json              WebContext::decode_json() const
    {
        if(rx_body.empty())
            return nlohmann::json();
        return nlohmann::json::parse(rx_body.begin(), rx_body.end());
    }


    //! Decodes post parameters
    StringViewMultiMap          WebContext::decode_post() const
    {
        if((rx_content_type != ContentType()) && (rx_content_type != ContentType::form))
            return StringViewMultiMap();
        return parse_parameters(rx_body);
    }

    StringViewMultiMap          WebContext::decode_query() const
    {
        return parse_parameters(url.query);
    }

    const Root*         WebContext::def_root(DataRole dr) const
    {
        if(session.def_root)
            return session.def_root;
        return wksp::root_reads(dr).value(0, nullptr);
    }
    

    std::string         WebContext::find_query(std::string_view k) const
    {
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
        return vsplit(url.query, '&', [&](std::string_view b) -> std::string {
            const char* eq  = strnchr(b, '=');
            if(!eq)
                return std::string();
            if(!is_in(std::string_view(b.data(), eq), keys))
                return std::string();
            return web_decode(std::string_view(eq+1, b.end()));
        });
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

    QueryStripped       WebContext::strip_query(std::string_view k, bool first) const
    {
        return stripped_query(k, url.query, first);
    }
}
