#include "Curl.hpp"
#include <curl/curl.h>
#include <QJsonDocument>

namespace yq {

    size_t   Curl::rx_bytes_impl(char* p, size_t, size_t sz, Curl* c)
    {
        if(p && sz && c)
            c -> m_rx.append((const uint8_t*) p, sz);
        return sz;
    }

    Curl::Curl()
    {
        m_curl  = curl_easy_init();
        curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, Curl::rx_bytes_impl);
        curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, this);
    }

    Curl::~Curl()
    {
        curl_easy_cleanup(m_curl);
    }

    HttpStatus          Curl::exec()
    {
        m_rx.clear();
        if(curl_easy_perform(m_curl) == CURLE_OK){
            long    v = 0;
            curl_easy_getinfo(m_curl, CURLINFO_RESPONSE_CODE, &v);
            return HttpStatus( (HttpStatus::enum_t) v);
        } else
            return HttpStatus::CurlError;
    }


    String              Curl::get_effective_url() const
    {
        char*   c;
        if(curl_easy_getinfo(m_curl, CURLINFO_EFFECTIVE_URL, &c) == CURLE_OK)
            return String(c);
        return String();
    }


    QByteArray          Curl::rx_bytes() const
    {
        return QByteArray((const char*) m_rx.data(), m_rx.size());
    }

    QJsonDocument       Curl::rx_json() const
    {
        return QJsonDocument::fromJson(rx_bytes());
    }

    void                Curl::set_url(const char*z)
    {
        curl_easy_setopt(m_curl, CURLOPT_URL, z);
    }

    void                Curl::set_url(const QUrl&u)
    {
        set_url(u.toString().toStdString());
    }

    void                Curl::set_url(const std::string&z)
    {
        set_url(z.c_str());
    }

}
