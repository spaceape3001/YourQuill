////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <tbb/spin_rw_mutex.h>
#include "WebPage.hpp"
#include "WebTemplate.hpp"

#include "WebRepo.hxx"
#include <yq/file/FileUtils.hpp>

namespace yq::mithril {

    WebRepo::WebRepo()
    {
        htmlTemplate    = new Template;
    }

    WebRepo& webRepo() 
    {
        static WebRepo s_ret;
        return s_ret;
    }



    namespace web {

        const std::vector<const WebPage*>&  all_pages()
        {
            return webRepo().allPages;
        }
        
        const std::vector<const WebVariable*>& all_variables()
        {
            return webRepo().allVars;
        }

        const WebPage*  directory(HttpOp m, std::string_view sv)
        {
            LOCK
            return _r.dirs[m].get(sv, nullptr);
        }
        
        const WebPageMap&    directory_map()
        {
            return webRepo().dirs;
        }

        const WebPage*  extension(HttpOp m, std::string_view sv)
        {
            LOCK
            return _r.exts[m].get(sv, nullptr);
        }

        void freeze()
        {
            WLOCK
            _r.openReg = false;
        }
        
        bool frozen()
        {
            LOCK
            return _r.openReg;
        }

        const WebPageMap&    glob_map()
        {
            return webRepo().globs;
        }
            
        const WebPageMap&    extension_map()
        {
            return webRepo().exts;
        }
        
        const WebPage* glob(HttpOp m, std::string_view sv)
        {
            LOCK
            return _r.globs[m].get(sv, nullptr);
        }

        Ref<Template>    html_template()
        {
            LLOCK
            return _r.htmlTemplate;
        }

        const WebPage* page(HttpOp m, std::string_view sv)
        {
            LOCK
            return _r.pages[m].get(sv, nullptr);
        }

        const WebPageMap&    page_map()
        {
            return webRepo().pages;
        }
        
        // Sets the template (will reload as necessary)
        bool                set_template(const std::filesystem::path& p)
        {
            std::string     data    = file_string(p);
            if(data.empty())    
                return false;
            Ref<Template>   ht  = new Template(std::move(data));
            {
                WLOCK
                std::swap(_r.htmlTemplate, ht);
            }
            return true;
        }
        
        // Sets the template to content
        bool                set_template(std::string_view sv)
        {
            if(sv.empty())
                return false;
            Ref<Template>   ht  = new Template(sv);
            {
                WLOCK
                std::swap(_r.htmlTemplate, ht);
            }
            return true;
        }

        void    set_template(Ref<Template> r)
        {
            WLOCK
            std::swap(_r.htmlTemplate, r);
        }

        const WebVariable*   variable(std::string_view k)
        {
            LOCK
            return _r.variables.get(k, nullptr);
        }
    
        const WebVarMap&    variable_map()
        {
            return webRepo().variables;
        }
    }
}
