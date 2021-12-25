////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <util/text/String.hpp>
#include <util/type/ResultFwd.hpp>
#include <filesystem>
#include <string_view>

class QUrl;
namespace yq {
    class Stream;

    struct Url {
        enum {
            SCHEME      = 0x1,
            HOST        = 0x2,
            PATH        = 0x4,
            USER        = 0x8,
            PWD         = 0x10,
            QUERY       = 0x20,
            FRAGMENT    = 0x40,
            PORT        = 0x80
        };

        String      scheme;
        String      host;
        String      path;
        String      user;
        String      pwd;
        String      query;
        String      fragment;
        int         port    = 0;
        
        static url_r    parse(const std::string_view&, unsigned int opts=~0U);
        static Url      file(const std::filesystem::path&);
        String          string(unsigned int opts=~0U) const;
        void            write_to(Stream&, unsigned int opts=~0U) const;
        
        static Url      fromQUrl(const QUrl&);
        QUrl            qUrl(unsigned int opts=~0U) const;
    };


    Stream& operator<<(Stream&, const Url&);
}
