////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "WebPage.hpp"
#include "WebRepo.hxx"
#include <yq/text/match.hpp>
#include <mithril/logging.hpp>

namespace yq::mithril {
    
    WebPage::WebPage(HttpOps _methods, std::string_view p, const std::source_location& sl) : MetaBase(p, sl)
    {
        set({Flag::WEB, Flag::PAGE});
        
        m_methods   = _methods;
        
        WLOCK
        if(_r.openReg){
            _r.allPages.push_back(this);
        }
    }
    
    WebPage::~WebPage()
    {
    }

    bool  WebPage::anonymouse_posting() const
    {
        return has(Flag::POST_ANON);
    }

    bool  WebPage::local_only() const
    {
        return has(Flag::LOCAL_ONLY);
    }
    
    bool  WebPage::login_required() const
    {
        return has(Flag::LOGIN_REQ);
    }
    
    bool  WebPage::no_expansion() const
    {
        return has(Flag::NO_EXPAND);
    }

    void    WebPage::seal()
    {
        if(is_sealed())
            return ;
            
        if(!has(Flag::DISABLE_REG)){
            std::string_view    p   = path();
            if((!p.empty()) && (m_role == Role())){
                switch(p[0]){
                case '/':
                    if(ends(p, "/*")){
                        p   = p.substr(0, p.size()-2);
                        m_role  = Role::Directory;
                    } else if(ends(p, "/**")){
                        p   = p.substr(0, p.size()-3);
                        m_role  = Role::Glob;
                    } else {
                        m_role  = Role::Page;
                    }
                    break;
                case '*':
                    if(starts(p, "*.")){
                        p   = p.substr(2);
                        m_role  = Role::Extension;
                    }
                    break;
                default:
                    mithrilWarning << "Page '" << name() << "' does NOT start with '*' or '/', bad registeration.";
                    break;
                }
            }
            
            WLOCK
            switch(m_role){
            case Role::Directory:
                for(HttpOp m : HttpOp::all_values())
                    if(m_methods.is_set(m))
                        _r.dirs[m][p]     = this;
                break;
            case Role::Extension:
                for(HttpOp m : HttpOp::all_values())
                    if(m_methods.is_set(m))
                        _r.exts[m][p]     = this;
                break;
            case Role::Glob:
                for(HttpOp m : HttpOp::all_values())
                    if(m_methods.is_set(m))
                        _r.globs[m][p]     = this;
                break;
            case Role::Page:
                for(HttpOp m : HttpOp::all_values())
                    if(m_methods.is_set(m)){
                        _r.pages[m][p]     = this;
                        for(std::string_view a : m_alts)
                            _r.pages[m][a]  = this;
                    }
                break;
            default:
                break;
            }
        }
        
        set(Flag::SEALED);
    }


    //  ----------------------------------------------------------------------------------------------------------------
    WebPage::Writer::Writer(WebPage*p) : MetaBase::Writer(p), m_page(p)
    {
    }

    WebPage::Writer::Writer(Writer&& mv) : MetaBase::Writer(std::move(mv)), m_page(mv.m_page)
    {
        mv.m_page = nullptr;
    }
    
    WebPage::Writer& WebPage::Writer::operator=(Writer&& mv)
    {
        if(this != &mv){
            if(m_page)
                m_page -> seal();
            m_page  = mv.m_page;
            mv.m_page   = nullptr;
        }
        return *this;
    }
    
    WebPage::Writer::~Writer()
    {
        if(m_page)
            m_page -> seal();
    }

    WebPage::Writer&  WebPage::Writer::alt_path(std::string_view k)
    {
        if(m_page)
            m_page -> m_alts.push_back(k);
        return *this;
    }

    WebPage::Writer&  WebPage::Writer::anon_post()
    {
        if(m_page)
            m_page -> set(Flag::POST_ANON);
        return *this;
    }

    WebPage::Writer&  WebPage::Writer::argument(std::string_view k, std::string_view d) 
    {
        if(m_page)
            m_page -> m_args.push_back( Arg{k, d});
        return *this;
    }
    
    WebPage::Writer&  WebPage::Writer::content(ContentTypes ct)
    {
        if(m_page){
            m_page -> m_content_types   |= ct;
        }
        return *this;
    }

    WebPage::Writer&  WebPage::Writer::description(std::string_view sv)
    {
        if(m_page)
            MetaBase::Writer::description(sv);
        return *this;
    }
    
    WebPage::Writer&  WebPage::Writer::disable_reg()
    {
        if(m_page)
            m_page -> set(Flag::DISABLE_REG);
        return *this;
    }
    
    
    WebPage::Writer&  WebPage::Writer::label(std::string_view sv)
    {
        if(m_page && !sv.empty())
            m_page -> m_label = sv;
        return *this;
    }

    WebPage::Writer&  WebPage::Writer::local()
    {
        if(m_page){
            m_page -> set(Flag::LOCAL_ONLY);
            if(!m_page->local_only())
                mithrilWarning << "Page " << m_page->path() << " failed to set local-only flag";
        }
        return *this;
    }

    WebPage::Writer&  WebPage::Writer::login()
    {
        if(m_page)
            m_page -> set(Flag::LOGIN_REQ);
        return *this;
    }
    
    
    WebPage::Writer&  WebPage::Writer::no_expand()
    {
        if(m_page)
            m_page -> set(Flag::NO_EXPAND);
        return *this;
    }

    WebPage::Writer&  WebPage::Writer::primary(ContentType ct)
    {
        if(m_page){
            m_page -> m_content_type    = ct;
            m_page -> m_content_types   |= ct;
        }
        return *this;
    }
    
    
    WebPage::Writer& WebPage::Writer::role(Role r)
    {
        if(m_page){
            m_page -> m_role = r;
        }
        return *this;
    }
    
    WebPage::Writer& WebPage::Writer::script(std::string_view sv)
    {
        if(m_page)
            m_page -> m_scripts.push_back(sv);
        return *this;
    }

    void    WebPage::Writer::set_group(const WebGroup*g) 
    {
        if(m_page)
            m_page -> m_group = g;
    }


    WebPage::Writer& WebPage::Writer::sub(std::string_view p, const WebPage*w)
    {
        if(w){
            sub(w->methods(), p, w);
        }
        return *this;
    }
    

    WebPage::Writer& WebPage::Writer::sub(HttpOps m, std::string_view p, const WebPage*w)
    {
        if(w && m_page){
            for(HttpOp h : HttpOp::all_values()){
                if(m.is_set(h)){
                    m_page -> set(Flag::HAS_SUBS);
                    m_page -> m_subs[h][p]  = w;
                }
            }
        }
        return *this;
    }

    void    WebGroup::add(WebPage*p)
    {
        if(p){
            pages.push_back(p);
            p->m_group = this;
        }
    }
    
    void    reg_webgroup(std::initializer_list<WebPage*> gdef)
    {
        if(!std::empty(gdef)){
            WebGroup*   group   = new WebGroup;
            for(WebPage* p : gdef)
                group -> add(p);
        }
    }
}
