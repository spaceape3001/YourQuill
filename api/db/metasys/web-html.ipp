////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace yq {

    WebActionMethodPageMap  remapToActionMethods(const WebPageMap&wm)
    {
        WebActionMethodPageMap  ret;
        for(HttpOp h : HttpOp::all_values()){
            for(auto& itr : wm[h])
                ret[itr.first][h] = itr.second;
        }
        return ret;
    }

    namespace html {
        void    dev_table(WebHtml& out, const WebPageMap& wm, bool links)
        {
            auto    t   = out.table();
            for(auto& i : remapToActionMethods(wm)){
                out << "<TR><TH align=\"left\">" << i.first << "</TH>";
                for(HttpOp h : HttpOp::all_values()){
                    out << "<TD>";
                    const WebPage*pg = i.second[h];
                    if(pg){
                        if(links && (h == hGet)){
                            out << "<a href=\"" << i.first << "\">" << h << "</a>";
                        } else 
                            out << h;
                        if(pg->local_only())
                            out << "<br><i>local</i>";
                    }
                    out << "</TD>";
                }
                out << "</TR>\n";
            }
        } 

        void    dev_table(WebHtml& out, const WebVarMap& wm)
        {
            auto    t   = out.table();
            for(auto& i : wm){
                out << "<TR><TH align=\"left\">" << i.first << "</TH><TD>" << i.second->description() << "</TD></TR>\n";
            }
        }
    }
}
