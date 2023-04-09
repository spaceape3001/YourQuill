////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <aether/kernel/bit/Context.hpp>
#include <aether/kernel/io/Strings.hpp>
#include <io/XmlUtils.hpp>
#include <io/StreamOps.hpp>
#include <io/stream/Text.hpp>

namespace yq {
    bool        read_xn(Context&ret, const XmlNode*xn)
    {
        ret.title   = read_attribute(xn, szTitle, x_string);
        ret.icon    = read_attribute(xn, szIcon, x_string);
        ret.order   = read_attribute(xn, szOrder, x_integer).value_or(0);
        ret.format  = read_attribute(xn, szFormat, x_enum<Format>).value_or(Format());
        ret.data    = to_string(xn);
        return true;
    }

    Context     x_context(const XmlNode* xn)
    {
        Context ret;
        read_xn(ret, xn);
        return ret;
    }

    void                write_xn(XmlNode* xn, const Context&v)
    {
        if(v.format != Format())
            write_attribute(xn, szFormat, v.format);
        if(!v.title.empty())
            write_attribute(xn, szTitle, v.title);
        if(!v.icon.empty())
            write_attribute(xn, szIcon, v.icon);
        if(v.order)
            write_attribute(xn, szOrder, v.order);
        if(!v.data.empty()){
            xn->append_node(
                xn->document()->allocate_node(
                    rapidxml::node_cdata, nullptr, v.data.c_str(), 0ULL, v.data.size()
                )
            );
        }
    }

    std::error_code       read_kv(std::vector<Context>& context, std::string_view body)
    {
        Context             cur;
        stream::Text        pipe(cur.data);
        
        vsplit(body, '\n', [&](std::string_view bit){
            if(starts(bit, "#!")){
                bit = trimmed(bit.substr(2));
                std::string_view    cmd, arg;

                size_t  i   = bit.find_first_of(" \t");
                if(i != std::string_view::npos){
                    cmd         = trimmed(bit.substr(0, i));
                    arg         = trimmed(bit.substr(i+1));
                } else 
                    cmd = bit;
                    
                if(is_similar(cmd, szContext)){
                    if(cur != Context()){
                        context.push_back(cur);
                        cur = Context();
                    }
                } else if(is_similar(cmd, szTitle)){
                    cur.title       = std::string(arg);
                } else if(is_similar(cmd, szFormat)){
                    cur.format      = Format(arg);
                } else if(is_similar(cmd, szOrder)){
                    cur.order       = to_uint(arg).value_or(0);
                } else if(is_similar(cmd, szIcon)){
                    cur.icon        = std::string(arg);
                } 
                
                //  No else, assumed to be an ill-mannered command
            } else {
                if(cur.data.empty() && (is_space(bit) || bit.empty()))
                    return ;
                if(is_space(bit))
                    pipe << '\n';
                else
                    pipe << bit << '\n';
            }
        });
        
        if(cur != Context())
            context.push_back(cur);
        return std::error_code();
    }
    
    void       write_kv(Stream& pipe, const std::vector<Context>& contexts)
    {
        bool    first   = true;
        for(const Context& ctx : contexts){
            if(first){
                first   = false;
            } else {
                pipe << "\n#! " << szContext << '\n';
            }
            
            if(!ctx.title.empty())
                pipe << "#! " << szTitle << '\t' << ctx.title << '\n';
            if(!ctx.icon.empty())
                pipe << "#! " << szIcon << '\t' << ctx.icon << '\n';
            if(ctx.order)
                pipe << "#! " << szOrder << '\t' << ctx.order << '\n';
            if(ctx.format != Format())
                pipe << "#! " << szFormat << '\t' << ctx.format.key() << '\n';
        }
    }
    
}
