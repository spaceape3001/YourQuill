////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "WebAdapters.hpp"
#include "WebContext.hpp"
#include <yq/file/FileUtils.hpp>

namespace yq::mithril {

    namespace {
       class SimpleWebFileHandler : public WebPage {
        public:
        
            SimpleWebFileHandler(HttpOps m, std::string_view p, const std::source_location& sl, const std::filesystem::path&f) : WebPage(m, p, sl), m_file(f) 
            {
                m_ext       = f.extension().string();
                if(!m_ext.empty() && m_ext[0] == '.')
                    m_ext   = m_ext.substr(1);
            }

            virtual void handle(WebContext&ctx) const override 
            {
                const WebPage*  x   = web::extension(hGet, m_ext);
                if(x){
                    x -> handle(ctx);
                } else {
                    send_file(m_file, ctx);
                }
            }
        
            std::filesystem::path       m_file;
            std::string                 m_ext;
        };
        
        class SimpleWebDirectory : public WebPage {
        public:
            SimpleWebDirectory(HttpOps m, std::string_view p,  const std::source_location& sl, const std::filesystem::path&f) : WebPage(m, p, sl), m_dir(f) 
            {
            }
            
            virtual void handle(WebContext&ctx) const override 
            {
                std::string     p   = path_sanitize(ctx.truncated_path);
                std::filesystem::path   fp  = m_dir / p;
                if(!std::filesystem::exists(fp)){
                    ctx.status = HttpStatus::NotFound;
                    return; 
                } 
                
                auto ext    = file_extension(p);
                if(ext.empty()){
                    send_file(fp, ctx);
                    return ;
                }
            
                const WebPage*  x   = web::extension(hGet, ext);
                if(x){
                    ctx.resolved_file  = fp;
                    x -> handle(ctx);
                } else {
                    send_file(fp, ctx);
                }
            }
        
            std::filesystem::path       m_dir;
        };
     
        class ManyWebDirectory : public WebPage {
        public:
            ManyWebDirectory(HttpOps m, std::string_view p,  const std::source_location& sl, const path_vector_t&f) : WebPage(m, p, sl), m_dirs(f) 
            {
            }
            
            virtual void handle(WebContext&ctx) const override 
            {
                std::string              p = path_sanitize(ctx.truncated_path);
                std::filesystem::path   fp = first(p);
                if(fp.empty()){
                    ctx.status = HttpStatus::NotFound;
                    return; 
                }

                auto ext    = file_extension(p);
                if(ext.empty()){
                    send_file(fp, ctx);
                    return ;
                }
            
                const WebPage*  x   = web::extension(hGet, ext);
                if(x){
                    ctx.resolved_file  = fp;
                    x -> handle(ctx);
                } else {
                    send_file(fp, ctx);
                }
            }
            
            std::filesystem::path       first(std::string_view v) const
            {
                for(auto& d : m_dirs){
                    std::filesystem::path   f   = d / v;
                    if(std::filesystem::exists(f))
                        return f;
                }
                
                return std::filesystem::path();
            }
            
            path_vector_t               m_dirs;
        };
    }

    WebPage::Writer     reg_webpage(const std::string_view& path, const std::filesystem::path&fp, const std::source_location& sl)
    {
        if(std::filesystem::is_directory(fp)){
            return WebPage::Writer( new SimpleWebDirectory(hGet, path, sl, fp));
        } else {
            return WebPage::Writer( new SimpleWebFileHandler(hGet, path, sl, fp));
        }
        
    }
    
    WebPage::Writer     reg_webpage(const std::string_view& path, const path_vector_t&dirs, const std::source_location& sl)
    {
        if(dirs.empty())
            return WebPage::Writer();
        return WebPage::Writer( new ManyWebDirectory(hGet, path, sl, dirs));
    }
    
    //  ------------------------------------------------

    namespace {
        class FunctionWebAdapter : public WebPage {
        public:
            FunctionWebAdapter(HttpOps _methods, std::string_view _path, const std::source_location& sl, std::function<void(WebContext&)> fn) :
                WebPage(_methods, _path, sl), m_fn(fn) {}
                
            virtual void handle(WebContext& ctx) const override
            {
                m_fn(ctx);
            }
            
            std::function<void(WebContext&)>    m_fn;
        };
    }

    WebPage::Writer     reg_webpage(std::string_view path, std::function<void(WebContext&)>fn, const std::source_location& sl)
    {
        return WebPage::Writer( new FunctionWebAdapter(hGet, path, sl, fn));
    }
    
    WebPage::Writer     reg_webpage(HttpOps methods, std::string_view path, std::function<void(WebContext&)>fn, const std::source_location& sl)
    {
        return WebPage::Writer( new FunctionWebAdapter(methods, path, sl, fn));
    }
    
    //  ------------------------------------------------
    namespace {
        class FunctionHtmlAdapter : public WebPage {
        public:
            FunctionHtmlAdapter(HttpOps _methods, std::string_view _path, std::string_view _title, const std::source_location& sl, std::function<void(WebHtml&)> fn) :
                WebPage(_methods, _path, sl), m_fn(fn), m_title(_title) {}
                
            virtual void handle(WebContext& ctx) const override
            {
                WebHtml out(ctx, m_title);
                m_fn(out);
            }
            
            std::function<void(WebHtml&)>       m_fn;
            std::string_view                    m_title;
        };
    }

    WebPage::Writer     reg_webpage(std::string_view path, std::function<void(WebHtml&)> fn, const std::source_location& sl)
    {
        return WebPage::Writer( new FunctionHtmlAdapter(hGet, path, std::string_view(), sl, fn));
    }
    
    WebPage::Writer     reg_webpage(HttpOps methods, std::string_view path, std::function<void(WebHtml&)>fn, const std::source_location& sl)
    {
        return WebPage::Writer( new FunctionHtmlAdapter(methods, path, std::string_view(), sl, fn));
    }
    
    WebPage::Writer     reg_webpage(std::string_view path, std::string_view title, std::function<void(WebHtml&)>fn, const std::source_location& sl)
    {
        return WebPage::Writer( new FunctionHtmlAdapter(hGet, path, title, sl, fn));
    }
    
    WebPage::Writer     reg_webpage(HttpOps methods, std::string_view path, std::string_view title, std::function<void(WebHtml&)>fn, const std::source_location& sl)
    {
        return WebPage::Writer( new FunctionHtmlAdapter(methods, path, title, sl, fn));
    }
}
