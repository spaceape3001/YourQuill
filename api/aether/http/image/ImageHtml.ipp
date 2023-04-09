////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ImageHtml.hpp"
#include <aether/http/file/FragmentHtml.hpp>
#include <aether/http/web/WebContext.hpp>
#include <aether/http/web/WebHtml.hpp>
#include <aether/kernel/file/DocumentCDB.hpp>
#include <aether/kernel/file/FragmentCDB.hpp>
#include <aether/kernel/image/ImageCDB.hpp>

namespace yq {
    namespace html {
        WebHtml&    operator<<(WebHtml& h, Image i)
        {
            std::string n   = cdb::label(cdb::document(i));
            h << "<img src=\"/image?id=" << i.id << "\" alt=\"";
            html_escape_write(h, n);
            h << "\">";
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, Dev<Image>v)
        {
            h << "<a href=\"/dev/image?id=" << v.data.id << "\">" << cdb::thumbnail(v.data) << "</a>";
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, DevID<Image> v)
        {
            h << "<a href=\"/dev/image?id=" << v.data.id << "\">" << v.data.id << "</a>";
            return h;
        }

        WebHtml&    operator<<(WebHtml&h, SizeDesc sz)
        {
            switch(sz){
            case SizeDesc::Original:
                h << "original";
                break;
            case SizeDesc::Large:
                h << "large";
                break;
            case SizeDesc::Medium:
                h << "medium";
                break;
            case SizeDesc::Small:
            default:
                h << "small";
                break;
            }
            return h;
        }

        
        WebHtml&    operator<<(WebHtml& h, const Thumbnail& t)
        {
            std::string n   = t.alt;
            if(n.empty())
                n           = "Thumbnail for '" + cdb::label(cdb::document(t.img)) + "'";
            cdb::label(cdb::document(t.img));
            h << "<img src=\"/thumbnail?id=" << t.img.id;
            //if(t.size != SizeDesc())
                //h << "&size=" << t.size.key();
            h << "\" alt=\"";
            html_escape_write(h, n);
            h << "\" class=\"" << t.size << "\"/>";
            return h;
        }

        void        dev_table(WebHtml& h, const std::vector<Image>& images)
        {
            auto t = h.table();
            h << "<tr><th>ID</th><th>Thumb</th><th>Fragment</th></tr>\n";
            for(Image i : images)
                h << "<tr><td>" << dev_id(i) << "</td><td>" << cdb::thumbnail(i, SizeDesc::Small) << "</td><td>" 
                    << dev(cdb::fragment(i)) << "</td></tr>\n";
        }
    }
}
