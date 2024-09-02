////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "VarTemplate.hpp"
#include "WebHtml.hpp"
#include <mithril/document/DocumentCDB.hpp>
#include <mithril/folder/FolderCDB.hpp>
#include <mithril/fragment/FragmentCDB.hpp>
#include <yq/io/FileUtils.hpp>
#include <mithril/notify/FileWatch.hpp>
#include <mithril/notify/FileNotifyAdapters.hpp>
#include <mithril/notify/Stage4.hpp>

namespace yq::mithril {

    VarTemplate::VarTemplate(std::string_view k, const std::filesystem::path&f, const std::source_location&sl) : WebVariable(k, sl), m_master(f)
    {
        on_stage4([this](){ this->update(); }, sl);
        if(!f.empty())
            on_watch(f, [this](){ this->update(); }, sl);
    }
    
    void    VarTemplate::update()
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
        m_text  = new Template(std::move(dat));
    }
    
    void    VarTemplate::handle(WebHtml&h) const 
    {
        Ref<Template> r = m_text;
        if(r)
            r->execute(h);
    }


    VarTemplate::Writer&     VarTemplate::Writer::source(std::string_view k, const std::source_location& sl)
    {
        return source(cdb::top_folder(), k, sl);
    }
    
    VarTemplate::Writer&     VarTemplate::Writer::source(Folder f, std::string_view k, const std::source_location& sl)
    {
        if(m_template){
            m_template->m_sources.push_back(FolderStr{f, k});
            VarTemplate *tmp    = m_template;
            on_change(by_cache(f, k), [tmp](){ tmp -> update(); }, sl);
        }
        return *this;
    }

    VarTemplate::Writer     reg_webvar(std::string_view k, const std::filesystem::path&f, const std::source_location& sl)
    {
        return VarTemplate::Writer(new VarTemplate(k, f, sl));
    }
}
