////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

//#include <mithril/enum/change.hpp>
#include <yq/trait/not_copyable.hpp>
#include <0/basic/EnumMap.hpp>
#include <0/meta/Meta.hpp>
#include <0/meta/MetaWriter.hpp>
#include <0/basic/Http.hpp>
#include <0/basic/Flag.hpp>

#include <mithril/preamble.hpp>

#include <functional>
#include <source_location>

namespace yq::mithril {
    struct Folder;
    class HttpRequest;
    class HttpResponse;
    
    struct WebSession;
    class Template;
    
    using HttpOps       = Flag<HttpOp>;
    using ContentTypes  = Flag<ContentType>;
    
    class WebPage;
    using WebPageMap    = EnumMap<HttpOp, Map<std::string_view, const WebPage*, IgCase>>;
    
    struct WebContext;
    
    
    namespace web {
        void freeze();
        bool frozen();
        
        // Not thread-safe until freeze() has been called.
        const std::vector<const WebPage*>&  all_pages();
        
        const WebPage*      page(HttpOp, std::string_view);
        const WebPage*      extension(HttpOp, std::string_view);
        const WebPage*      directory(HttpOp, std::string_view);
        const WebPage*      glob(HttpOp, std::string_view);

        // Not thread-safe until freeze() has been called.
        const WebPageMap&   page_map();

        // Not thread-safe until freeze() has been called.
        const WebPageMap&   extension_map();

        // Not thread-safe until freeze() has been called.
        const WebPageMap&   directory_map();

        // Not thread-safe until freeze() has been called.
        const WebPageMap&   glob_map();

        // Gets the current html template
        Ref<Template>    html_template();
        
        // Sets the template (will reload as necessary)
        bool                set_template(const std::filesystem::path&);
        // Sets the template to content
        bool                set_template(std::string_view);

    };
    
    struct WebGroup {
        std::vector<const WebPage*> pages;
        void    add(WebPage*);
    };

    /*! \brief WebPage (or series of pages)
    
        \property path   The path of the web page, as specified.  
            Wildcards can be used to control how it's registered.  
            Starting with '/' signifies it's a page (generally)  
            If the ending file of a page is '*', it's treated as a simple directory.
            A '**' is treated as a recursive directory structure.
            Startign with '*.' signifies it's a custom file handler for extension which 
            standard directory handlers will utilize.
    */
    class WebPage : public MetaBase {
        friend struct WebGroup;
    public:
    
        YQ_ENUM(Role, ,
            //! WebPage Role unknown/other
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
    
    
        /*! \brief Handles the request
            \note truncated_path has NOT been sanitized!
        */
        virtual void    handle(WebContext&) const = 0;
        
        //! TRUE if anonymous posting is allowed
        bool                        anonymouse_posting() const;
        
        //! List of arguments expected for GET/POST queries
        const std::vector<Arg>&     args() const { return m_args; }
        //! Primary content type
        ContentType                 content_type() const { return m_content_type; }
        //! All general content types
        ContentTypes                content_types() const { return m_content_types; }
        std::string_view            label() const { return m_label; }
        bool                        local_only() const;
        bool                        login_required() const;
        bool                        no_expansion() const;
        std::string_view            path() const { return name(); }
        Role                        role() const { return m_role; }
        HttpOps                     methods() const { return m_methods; }
        const WebGroup*             group() const { return m_group; }
        const std::vector<std::string_view>&    scripts() const { return m_scripts; }
        
        
    protected:
        WebPage(HttpOps, std::string_view, const std::source_location&);
        ~WebPage();
        
        void                    set_path(std::string_view v) { set_name(v); }
        
        std::vector<Arg>        m_args;
        std::vector<std::string_view>   m_scripts;
        WebPageMap              m_subs;     // for derived use... (ie, overrides or whatever)
        std::string_view        m_label;
        const WebGroup*         m_group = nullptr;
        HttpOps                 m_methods;
        ContentTypes            m_content_types;
        ContentType             m_content_type;
        Role                    m_role;
        std::vector<std::string_view>   m_alts;
        
        
        void                    seal();
    };

    struct WebPage::Arg {
        std::string_view  name;
        std::string_view  description;
    };
    
    class WebPage::Writer : public MetaBase::Writer, public yq::not_copyable {
    public:
        
        //! Allows for an alternate path (if we're pure-page no extensions)
        Writer&  alt_path(std::string_view);

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
        
        //! Sets the label for the tab-bar
        Writer&  label(std::string_view);
        
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
        
        //! Adds a script
        Writer& script(std::string_view);
        
        //! Adds in a sub-web, overriding the web's path
        Writer&  sub(std::string_view, const WebPage*);

        //! Adds in a sub-web, overriding the web's path & methods
        Writer&  sub(HttpOps, std::string_view, const WebPage*);
        
        //! Sets the group of the page
        void     set_group(const WebGroup*);
        
        //Writer&  on_change(const std::filesystem::path&, const std::source_location& sl = std::source_location::current());
        //Writer&  on_change(std::string_view ext, const std::source_location& sl = std::source_location::current());
        //Writer&  on_change(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current());
        //Writer&  on_stage4(int order=0, const std::source_location& sl = std::source_location::current());
        
        const WebPage*  page() const { return m_page; }
        
        
        Writer(WebPage*p); // : m_page(p) {}
        Writer(Writer&&);
        Writer& operator=(Writer&&);
        ~Writer();
        Writer() : MetaBase::Writer(nullptr) {}
        
        operator WebPage* () const { return m_page; }
        
    private:
        WebPage*   m_page = nullptr;
    };
    
    
    bool    send_file(const std::filesystem::path&, WebContext&, ContentType ct=ContentType());
    bool    send_file_info(const std::filesystem::path&, WebContext&, ContentType ct=ContentType());
}
