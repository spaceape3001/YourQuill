////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/graphviz/GraphvizBuilder.hpp>
#include <yq/mithril/graphviz/GraphvizCDB.hpp> // placebo to get cdb::key & name into scope
#include <0/basic/TextUtils.hpp>
#include <variant>
#include <functional>
#include <vector>

namespace yq::mithril {
    template <IdType S>
    struct IdGrapher  {
        
        using IdEdge        = std::pair<S,S>;
        using StrFunction   = std::function<std::string(S)>;
        using StrFunction2  = std::function<std::string(S,S)>;
        using FlexString    = std::variant<std::monostate,std::string,StrFunction>;
        using FlexString2   = std::variant<std::monostate,std::string,StrFunction2>;
        
        
        std::vector<S>      nodes;
        std::vector<IdEdge> edges;
        S                   focus;
        std::string         background;     // color.... for entire image
        std::string         rank            = "RL";
        bool                nameWithKey     = true;
        FlexString          url;
        FlexString          textColor;      // Text color
        FlexString          boxColor;       // Box backgrounds
        FlexString2         lineColor;
        
        void                build_to(Graphviz::Builder& ret) const
        {
            ret.m_rank      = rank;
            ret.m_bgcolor   = background;
            
            const std::string* const  url_s     = std::get_if<std::string>(&url);
            const StrFunction* const  url_fn    = std::get_if<StrFunction>(&url);
            
            const std::string*  const text_s    = std::get_if<std::string>(&textColor);
            const StrFunction*  const text_fn   = std::get_if<StrFunction>(&textColor);
            
            const std::string*  const box_s     = std::get_if<std::string>(&boxColor);
            const StrFunction*  const box_fn    = std::get_if<StrFunction>(&boxColor);

            const std::string*  const line_s    = std::get_if<std::string>(&lineColor);
            const StrFunction2* const line_fn   = std::get_if<StrFunction2>(&lineColor);

            for(S c : nodes){
                std::string k   = cdb::key(c);
                std::string t   = cdb::name(c);
                if(t.empty())
                    t   = k;
                
                std::string kk;
                if(nameWithKey){
                    kk      = "k_" + k;
                } else {
                    kk      = "k_";
                    kk += to_integer(c.id);
                }
                
                auto& n = ret.node("k_" + k, t);

                if(url_s){
                    if((c != focus) && !url_s->empty())
                        n.url   = (*url_s) + k;
                }

                if(url_fn){
                    n.url       = (*url_fn)(c);
                }

                if(text_s)
                    n.fgcolor   = *text_s;
                if(text_fn)
                    n.fgcolor   = (*text_fn)(c);

                if(box_s)
                    n.bgcolor   = *box_s;
                if(box_fn)
                    n.bgcolor   = (*box_fn)(c);
                
                if((c==focus) && text_s && box_s)
                    std::swap(n.bgcolor, n.fgcolor);
            }
            
            for(auto& p : edges){
                auto& e = ret.edge("k_" + cdb::key(p.first), "k_" + cdb::key(p.second));
                
                if(line_s)
                    e.color = *line_s;
                if(line_fn)
                    e.color = (*line_fn)(p.first, p.second);
            }
        }
        
        std::string         make_dot(std::string_view name="idGraph") const
        {
            Graphviz::Builder   gb(name);
            build_to(gb);
            return gb.make_dot();
        }
        
        Graphviz            make_graph(std::string_view name="idGraph") const
        {
            Graphviz::Builder   gb(name);
            build_to(gb);
            return gb.build();
        }
    };

}

