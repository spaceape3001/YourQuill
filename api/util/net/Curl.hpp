////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <util/net/Http.hpp>
#include <util/text/String.hpp>


class QUrl;
class QJsonDocument;

namespace yq {

    class Curl {
    public:
        Curl();
        ~Curl();
        
        
        String              get_effective_url() const;
        
        void                set_url(const QUrl&);
        void                set_url(const std::string&);
        void                set_url(const char*);
        
        using JsonResult    = std::pair<QJsonDocument,HttpStatus>;
        using UInt8Result   = std::pair<Vector<uint8_t>,HttpStatus>;
        using BytesResult   = std::pair<QByteArray,HttpStatus>;
        
        HttpStatus          exec();
        
        const Vector<uint8_t>&      rx() const { return m_rx; }
        QByteArray                  rx_bytes() const;
        QJsonDocument               rx_json() const;
        
    private:
        Curl(const Curl&) = delete;
        Curl(Curl&&) = delete;
        Curl& operator=(const Curl&) = delete;
        Curl& operator=(Curl&&) = delete;
        
        void*               m_curl;
        Vector<uint8_t>     m_rx;
        
        static size_t   rx_bytes_impl(char*, size_t, size_t, Curl*);
        
    };
}
