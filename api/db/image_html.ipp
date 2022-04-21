////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

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
