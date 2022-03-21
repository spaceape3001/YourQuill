////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <util/meta/Meta.hpp>
#include <util/meta/MetaWriter.hpp>
#include <util/net/Http.hpp>

#include <functional>
#include <source_location>

namespace yq {
    class HttpRequest;
    class HttpResponse;

    class WebPage : public Meta {
    public:
    
        class Writer;
        struct Arg;
    
        static const std::vector<const WebPage*>&  all();
        
        static void freeze();
        static bool frozen();
    
        static const WebPage*      find(HttpOp, const std::string_view&);
    
        virtual void    handle(const HttpRequest&, HttpResponse&) const = 0;
        
        bool                        anonymouse_posting() const;
        const std::vector<Arg>&     args() const { return m_args; }
        const std::string_view&     description() const { return m_description; }
        bool                        local_only() const;
        bool                        login_required() const;
        bool                        no_expansion() const;
        const std::string_view&     path() const { return m_path; }
        HttpOp                      method() const { return m_method; }
        const std::source_location& source() const { return m_source; }
        
    protected:
        WebPage(HttpOp, std::string_view, const std::source_location&);
        ~WebPage();
        
        std::vector<Arg>        m_args;
        std::string_view        m_description;
        std::string_view        m_path;
        std::source_location    m_source;
        HttpOp                  m_method;
        unsigned int            m_flags = 0;
        
        enum {
            LOCAL_ONLY      = 0x1,
            LOGIN_REQ       = 0x2,
            NO_EXPAND       = 0x4,
            POST_ANON       = 0x8
        };
        
        struct Repo;
        static Repo&        repo();
    };

    struct WebPage::Arg {
        std::string_view  name;
        std::string_view  description;
    };
    
    class WebPage::Writer : public Meta::Writer {
    public:
        
        Writer&  login();
        Writer&  description(std::string_view);
        Writer&  argument(std::string_view k, std::string_view d=std::string_view());
        Writer&  local();
        Writer&  anon_post();
        Writer&  no_expand();
        
        Writer(WebPage*p); // : m_page(p) {}
    private:
        WebPage*   m_page = nullptr;
    };
    
//    WebPage::Writer    reg_webpage(std::string_view, const std::filesystem::path&, const std::source_location& sl = std::source_location::current());
    WebPage::Writer    reg_webpage(std::string_view, std::function<void(const HttpRequest&, HttpResponse&)>, const std::source_location& sl = std::source_location::current());
    WebPage::Writer    reg_webpage(HttpOp, std::string_view, std::function<void(const HttpRequest&, HttpResponse&)>, const std::source_location& sl = std::source_location::current());
}
