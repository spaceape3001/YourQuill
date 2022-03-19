////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <util/preamble.hpp>

namespace yq {
    class Stream;

    template <typename T>
    struct BasicUrl {
        T       scheme;
        T       user;
        T       pwd;
        T       host;
        T       path;
        T       query;
        T       fragment;
        int     port    = 0;
        

    //! Returns a string view based URL 
    //! \note Original URL MUST LIVE!
        operator BasicUrl<std::string_view> () const 
        requires std::is_same_v<T,std::string>
        {
            return { scheme, user, pwd, host, path, query, fragment, port };
        }

        bool    operator==(const BasicUrl&) const = default;
    };

    Url             copy(const UrlView&);
    url_view_r      to_url(const char*, size_t);
    url_view_r      to_url(const char*);
    url_view_r      to_url(const std::string_view&);
    Url             to_url(const std::filesystem::path&);
    
    url_view_r      as_host_port(const char*, size_t);
    url_view_r      as_host_port(const std::string_view&);
    
    //! Tests for a valid URL
    //! \note empty scheme is treated as valid for this context
    bool            is_valid(const UrlView&);

    Stream&         operator<<(Stream&, const Url&);
    Stream&         operator<<(Stream&, const UrlView&);
    
    std::ostream&   operator<<(std::ostream&, const Url&);
    std::ostream&   operator<<(std::ostream&, const UrlView&);

    //log4cpp::CategoryStream&         operator<<(log4cpp::CategoryStream&, const Url&);
    //log4cpp::CategoryStream&         operator<<(log4cpp::CategoryStream&, const UrlView&);
    

}
