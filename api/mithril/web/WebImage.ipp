////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "WebImage.hpp"

#include <mithril/document/DocumentCDB.hpp>
#include <mithril/folder/FolderCDB.hpp>
#include <mithril/fragment/Fragment.hpp>
#include <mithril/image/Image.hpp>
#include <mithril/notify/FileWatch.hpp>
#include <mithril/notify/FileNotifyAdapters.hpp>
#include <mithril/notify/Stage4.hpp>
#include <yq/core/StreamOps.hpp>
#include <yq/stream/Text.hpp>

namespace yq::mithril {
    WebImage::WebImage(std::string_view kpath, const std::filesystem::path& f0, Folder folder, std::string_view prefix, const std::source_location&sl) : 
        WebPage(hGet, kpath, sl), 
        m_fallback(f0)
    {
        m_fallback  = f0;
        if(!folder)
            folder  = cdb::top_folder();
        m_parent    = folder;

        for(const char* z : Image::kSupportedExtensions)
            m_files.push_back(std::string(prefix) +'.' + z);
        
        on_stage4([this](){ this->update(); }, sl);
        
        if(!f0.empty())
            on_change(by_file(f0), [this](){ this -> update(); }, sl);
        
        for(std::string_view s : m_files){
            on_change(by_cache(folder, s), [this](){ this->update(); }, sl);
        }
    }
    
    void    WebImage::handle(WebContext&ctx) const 
    {
        Ref<TypedBytes>     data = m_data;
        if(!data){
            ctx.status  = HttpStatus::ServiceUnavailable;
            return;
        }
        
        ctx.tx_content_type = data -> type;
        ctx.tx_content      = data -> data;
        ctx.tx_header("Date", data->modified);
        ctx.status          = HttpStatus::Success;
    }

    void    WebImage::update()
    {
        Document        doc = cdb::first_document(m_parent, m_files);
        Ref<TypedBytes> data;
        if(doc){
            do {
                Fragment    frag = cdb::fragment(doc, DataRole::Image);
                if(!frag)
                    break;
                std::filesystem::path   fp  = cdb::path(frag);
                data  = TypedBytes::load(fp);
                if(!data)
                    break;
            } while(false);
        } else if(!m_fallback.empty()){
            data        = TypedBytes::load(m_fallback);
        } 
        
        m_loaded    = data;
        m_data      = data;

        if(m_post)
            m_post(*this);
    }

    WebImage::Writer&         WebImage::Writer::post(std::function<void(WebImage&)> fn)
    {
        if(fn && m_image){
            m_image -> m_post = fn;
        }
        return *this;
    }

    WebImage::Writer    reg_webimage(std::string_view k, const std::filesystem::path& fp, Folder fo, std::string_view pfx, const std::source_location& sl)
    {
        return WebImage::Writer(new WebImage(k, fp, fo, pfx, sl));
    }

}
