////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "WebHtml.hpp"
#include "WebTemplate.hpp"
#include <kernel/file/DocumentCDB.hpp>
#include <kernel/file/FolderCDB.hpp>
#include <kernel/file/FragmentCDB.hpp>
#include <basic/FileUtils.hpp>
#include <kernel/notify/FileNotifyAdapters.hpp>
#include <kernel/notify/FileWatch.hpp>
#include <kernel/notify/Stage4.hpp>

namespace yq {

    WebTemplate::WebTemplate(std::string_view p, const std::filesystem::path&f, const std::source_location&sl) : 
        WebPage(hGet, p, sl), m_master(f)
    {
        on_stage4([this](){ this->update(); }, sl);
        if(!f.empty())
            on_watch(f, [this](){ this->update(); }, sl);
    }
    
    void        WebTemplate::update()
    {
        std::string dat;
        for(auto& s : m_sources){
            Document    doc = cdb::child_document(s.first, s.second);
            if(!doc)
                continue;
                
            Fragment    f  = cdb::first_fragment(doc);
            if(!f)
                continue;
                
            dat = cdb::frag_string(f, cdb::DONT_LOCK);
            if(!dat.empty())
                break;
        }
        
        if(dat.empty())
            dat = file_string(m_master);
        m_body  = new PageTemplate(std::move(dat));
    }
    
    void        WebTemplate::handle(WebContext& ctx) const 
    {
        Ref<PageTemplate>   r   = m_body;
        if(!r)
            return ;
            
        WebHtml h(ctx, "");
        r->execute(h);
    }

    WebTemplate::Writer&     WebTemplate::Writer::source(std::string_view sv, const std::source_location& sl)
    {
        source(cdb::top_folder(), sv, sl);
        return *this;
    }
    
    WebTemplate::Writer&     WebTemplate::Writer::source(Folder f, std::string_view sv, const std::source_location& sl)
    {
        if(m_template){
            m_template->m_sources.push_back(FolderStr{f, sv});
            WebTemplate *tmp    = m_template;
            on_change(by_cache(f, sv), [tmp](){ tmp -> update(); }, sl);
        }
        return *this;
    }
    
    WebTemplate::Writer     reg_webtemplate(std::string_view p, const std::filesystem::path&fp, const std::source_location& sl)
    {
        return WebTemplate::Writer(new WebTemplate(p, fp, sl));
    }
}
