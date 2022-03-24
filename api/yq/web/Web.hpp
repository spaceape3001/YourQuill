////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/collection/EnumMap.hpp>
#include <yq/meta/Meta.hpp>
#include <yq/meta/MetaWriter.hpp>
#include <yq/net/Http.hpp>
#include <yq/type/Flag.hpp>

#include <functional>
#include <source_location>

namespace yq {
    class HttpRequest;
    class HttpResponse;
    
    using HttpOps       = Flag<HttpOp>;
    using ContentTypes  = Flag<ContentType>;
    
    class Web;
    using WebMap    = EnumMap<HttpOp, Map<std::string_view, const Web*, IgCase>>;
    

    /*! \brief Web (or series of pages)
    
        \property path   The path of the web page, as specified.  
            Wildcards can be used to control how it's registered.  
            Starting with '/' signifies it's a page (generally)  
            If the ending file of a page is '*', it's treated as a simple directory.
            A '**' is treated as a recursive directory structure.
            Startign with '*.' signifies it's a custom file handler for extension which 
            standard directory handlers will utilize.
    */
    class Web : public Meta {
    public:
    
        YQ_ENUM(Role, ,
            //! Web Role unknown/other
            Unknown = 0,
            //! It's a web-page     /path/to/resource
            Page,
            //! Extension handler   *.extension
            Extension,
            //! Directory handler   /path/to/*
            Directory,
            //! Recursive Directory handler /path/to/**
            Glob
        );
    
        class Writer;
        struct Arg;
    
        static const std::vector<const Web*>&  all();
        
        static void freeze();
        static bool frozen();
    
        static const Web*       page(HttpOp, std::string_view);
        static const Web*       extension(HttpOp, std::string_view);
        static const Web*       directory(HttpOp, std::string_view);
        static const Web*       glob(HttpOp, std::string_view);
        
        static const WebMap&    directory_map();
        static const WebMap&    extension_map();
        static const WebMap&    page_map();
        static const WebMap&    glob_map();
        
    
    
        /*! \brief Handles the request
            \note truncated_path has NOT been sanitized!
        */
        virtual void    handle(const HttpRequest&, HttpResponse&, std::string_view truncated_path) const = 0;
        
        bool                        anonymouse_posting() const;
        const std::vector<Arg>&     args() const { return m_args; }
        //! Primary content type
        ContentType                 content_type() const { return m_content_type; }
        //! All general content types
        ContentTypes                content_types() const { return m_content_types; }
        bool                        local_only() const;
        bool                        login_required() const;
        bool                        no_expansion() const;
        std::string_view            path() const { return m_path; }
        Role                        role() const { return m_role; }
        HttpOps                     methods() const { return m_methods; }
        const std::source_location& source() const { return m_source; }
        
    protected:
        Web(HttpOps, std::string_view, const std::source_location&);
        ~Web();
        
        std::vector<Arg>        m_args;
        std::string_view        m_path;
        std::source_location    m_source;
        WebMap                  m_subs;     // for derived use... (ie, overrides or whatever)
        HttpOps                 m_methods;
        ContentTypes            m_content_types;
        ContentType             m_content_type;
        Role                    m_role;
        
        enum {
            LOCAL_ONLY      = 1ULL << 63,
            LOGIN_REQ       = 1ULL << 62,
            NO_EXPAND       = 1ULL << 61,
            POST_ANON       = 1ULL << 60,
            HAS_SUBS        = 1ULL << 59,
            DISABLE_REG     = 1ULL << 58,
            SEALED          = 1ULL << 57
        };
        
        void                    seal();
        
        struct Repo;
        static Repo&        repo();
    };

    struct Web::Arg {
        std::string_view  name;
        std::string_view  description;
    };
    
    class Web::Writer : public Meta::Writer {
    public:
        
        //! Allow POST to not require login.  (Needed for the login post)
        Writer&  anon_post();

        //! Annotates an argument (note, not enforced, for the help)
        Writer&  argument(std::string_view k, std::string_view d=std::string_view());
        
        //! Adds the additional content types
        Writer&  content(ContentTypes);

        //! Sets the description
        Writer&  description(std::string_view);
        
        //! Disable registration
        Writer&  disable_reg();
        
        //! Local address only (ie, same machine)
        Writer&  local();
        
        //! Sets this to login-required
        Writer&  login();
        
        //! Don't auto-expand the HTML....
        Writer&  no_expand();
        
        //! Sets the primary content type
        Writer&  primary(ContentType);
        
        /*! \brief Override the role (note, won't affect the auto-register)
            \note This will disable the stripping of "/" or "*".
        */
        Writer&  role(Role);
        
        //! Adds in a sub-web, overriding the web's path
        Writer&  sub(std::string_view, const Web*);

        //! Adds in a sub-web, overriding the web's path & methods
        Writer&  sub(HttpOps, std::string_view, const Web*);
        
        
        Writer(Web*p); // : m_page(p) {}
        Writer(Writer&&);
        Writer& operator=(Writer&&);
        ~Writer();
        
        operator const Web* () const { return m_page; }
        
        Writer(const Writer&) = delete;
        Writer& operator=(const Writer&&) = delete;
    private:
        Web*   m_page = nullptr;
    };
    
    bool            send_file(const std::filesystem::path&, HttpResponse&, ContentType ct=ContentType());
    bool            send_file_info(const std::filesystem::path&, HttpResponse&, ContentType ct=ContentType());
    
    //! Registers a file or a directory to the specified path ... does get & head ops
    Web::Writer     reg_web(std::string_view, const std::filesystem::path&, const std::source_location& sl = std::source_location::current());
    Web::Writer     reg_web(std::string_view, const path_vector_t&, const std::source_location& sl = std::source_location::current());

    Web::Writer     reg_web(std::string_view, std::function<void(const HttpRequest&, HttpResponse&)>, const std::source_location& sl = std::source_location::current());
    Web::Writer     reg_web(HttpOps, std::string_view, std::function<void(const HttpRequest&, HttpResponse&)>, const std::source_location& sl = std::source_location::current());

    Web::Writer     reg_web(std::string_view, std::function<void(const HttpRequest&, HttpResponse&, std::string_view)>, const std::source_location& sl = std::source_location::current());
    Web::Writer     reg_web(HttpOps, std::string_view, std::function<void(const HttpRequest&, HttpResponse&, std::string_view)>, const std::source_location& sl = std::source_location::current());

}
