////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/fragment/FragmentCDB.hpp>

#include <yq/container/Map.hpp>
#include <yq/core/StreamOps.hpp>
#include <yq/stream/Bytes.hpp>
#include <yq/text/chars.hpp>
#include <yq/text/vsplit.hpp>

namespace {
    void    css__(cdb_options_t opts=0)
    {
        using namespace yq;
        using namespace yq::mithril;
    
        std::string       css;
        for(Fragment f : cdb::fragments(".css", DataRole::Style)){
            css += cdb::frag_string(f, opts);
            css += '\n';
        }
            
        ByteArray           newCssData;
        {
            stream::Bytes       ncss(newCssData);
            
            std::string     newTxtColor = "black";
            std::string     newBkColor  = "white";

            Map<std::string, std::string_view, IgCase> vars;
            bool    inBody  = false;
            vsplit(css, '\n', [&](std::string_view l){
                if(l.empty())
                    return;
                    
                l = trimmed(l);
                if(inBody){
                    if(l[0] == '}'){
                        inBody  = false;
                        return;
                    }
                    size_t i  = l.find_first_of(':');
                    if(i == std::string_view::npos)
                        return;
                    std::string       k     = to_lower(trimmed(l.substr(0,i)));
                    std::string_view  v     = trimmed(l.substr(i+1));
                    
                    
                    if(is_similar(k, "color"))
                        newTxtColor      = v;
                    if(is_similar(k, "background-color"))
                        newBkColor      = v;
                    vars[k]     = v;
                } else if(starts_igCase(l, "body")) {
                    inBody  = true;
                }
            });
            
            gTextColor  = newTxtColor;
            gBkColor    = newBkColor;
            
            ncss << css;
            
            if(!vars.empty()){
                ncss << "\n/*\n    Variables auto-picked from existing CSS.\n*/\n";
                ncss << ":root {\n";
                for(auto& j : vars)
                    ncss << "    --" << j.first << ": " << j.second << '\n';
                ncss << "}\n\n";
            }
            if(gHasBackground){
                ncss << "body {\n"
                        "    background-image: url(\"/background\");\n"
                        "}\n";
            }
            
            ncss << file_string(gSharedCssFile);
        }
        gCss        = std::make_shared<ByteArray>(std::move(newCssData));
    }

    void    css_stage4()
    {
        css__(cdb::DONT_LOCK);
    }

    void    css_update()
    {
        css__();
    }
}
