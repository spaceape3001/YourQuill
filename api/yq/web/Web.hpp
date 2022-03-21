////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/meta/Meta.hpp>
#include <yq/meta/MetaWriter.hpp>
#include <yq/net/Http.hpp>
#include <yq/type/Flag.hpp>

#include <functional>
#include <source_location>

namespace yq {
    class HttpRequest;
    class HttpResponse;

    //! \brief Web (or series of pages)
    //!
    //!    \property path  
    //!      This is the path of the web page, as specified.  Wildcards at the end are significant.  
    //!      "dir/" is the directory itself (acceptable)
    //!      "dir/*" wildcards to anything in that directory
    //!      "dir/**" wildcards to that directory and descendents.
    class Web : public Meta {
    public:
    
        class Writer;
        struct Arg;
    
        static const std::vector<const Web*>&  all();
        
        static void freeze();
        static bool frozen();
    
        static const Web*      find(HttpOp, const std::string_view&);
    
        /*! \brief Handles the request
            \note truncated_path has NOT been sanitized!
        */
        virtual void    handle(const HttpRequest&, HttpResponse&, const std::string_view& truncated_path) const = 0;
        
        bool                        anonymouse_posting() const;
        const std::vector<Arg>&     args() const { return m_args; }
        const std::string_view&     description() const { return m_description; }
        bool                        is_directory() const;
        bool                        is_recursive() const;
        bool                        local_only() const;
        bool                        login_required() const;
        bool                        no_expansion() const;
        const std::string_view&     path() const { return m_path; }
        Flag<HttpOp>                methods() const { return m_method; }
        const std::source_location& source() const { return m_source; }
        
    protected:
        Web(Flag<HttpOp>, std::string_view, const std::source_location&);
        ~Web();
        
        std::vector<Arg>        m_args;
        std::string_view        m_description;
        std::string_view        m_path;
        std::source_location    m_source;
        Flag<HttpOp>            m_method;
        unsigned int            m_flags = 0;
        
        enum {
            LOCAL_ONLY      = 0x1,
            LOGIN_REQ       = 0x2,
            NO_EXPAND       = 0x4,
            POST_ANON       = 0x8,
            DIRECTORY       = 0x10,
            RECURSIVE       = 0x20
        };
        
        struct Repo;
        static Repo&        repo();
    };

    struct Web::Arg {
        std::string_view  name;
        std::string_view  description;
    };
    
    class Web::Writer : public Meta::Writer {
    public:
        
        Writer&  login();
        Writer&  description(std::string_view);
        Writer&  argument(std::string_view k, std::string_view d=std::string_view());
        Writer&  local();
        Writer&  anon_post();
        Writer&  no_expand();
        
        Writer(Web*p); // : m_page(p) {}
    private:
        Web*   m_page = nullptr;
    };

    //! Registers a file or a directory to the specified path
    Web::Writer     reg_web(std::string_view, std::filesystem::path&, const std::source_location& sl = std::source_location::current());

    Web::Writer     reg_web(std::string_view, std::function<void(const HttpRequest&, HttpResponse&)>, const std::source_location& sl = std::source_location::current());
    Web::Writer     reg_web(HttpOp, std::string_view, std::function<void(const HttpRequest&, HttpResponse&)>, const std::source_location& sl = std::source_location::current());

    Web::Writer    reg_web(std::string_view, std::function<void(const HttpRequest&, HttpResponse&, const std::string_view&)>, const std::source_location& sl = std::source_location::current());
    Web::Writer    reg_web(HttpOp, std::string_view, std::function<void(const HttpRequest&, HttpResponse&, const std::string_view&)>, const std::source_location& sl = std::source_location::current());

    
    bool            send_file(const std::filesystem::path&, HttpResponse&, ContentType ct=ContentType());
}
