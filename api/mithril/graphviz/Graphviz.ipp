////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/graphviz/Graphviz.hpp>
#include <mithril/wksp/Workspace.hpp>
#include <yq/io/Execute.hpp>
#include <yq/basic/ErrorDB.hpp>

namespace yq::mithril {
    namespace {
        std::filesystem::path   find_dot()
        {
            const std::filesystem::path&    s_dot   = wksp::dot();
            if(std::filesystem::exists(s_dot))
                return s_dot;
            const char* z   = getenv("GRAPHVIZ_PATH");
            if(!z)
                z   = "/usr";
            return std::filesystem::path(z) / "bin" / "dot";
        }
        
        const std::filesystem::path&    dot_exe()
        {
            static std::filesystem::path    s_ret   = find_dot();
            return s_ret;
        }
        
        std::pair<std::string,std::error_code>     dot_compile(std::string_view dotData, std::initializer_list<std::string_view> args)
        {
            int ecode   = -1;
            ProcessDescriptor pd;
            pd.args.push_back(dot_exe().string());
            for(std::string_view a : args)
                pd.args.push_back(std::string(a));
            ByteArray   input(dotData.data(), dotData.size());
            ByteArray   capture = pd.execute(input, nullptr, &ecode);
            std::string ret(capture.data(), capture.size());
            if(ecode != 0)
                return { ret, create_error<"Unable to compile DOT file">() };
            return { ret, std::error_code() };
        }
    }


    std::string      Graphviz::dot2html(std::string_view dd)
    {
        auto [s, ec] = dot_compile(dd, { "-Tcmapx" });
        return s;
    }
    
    std::string      Graphviz::dot2svg(std::string_view dd)
    {
        auto [s, ec] = dot_compile(dd, { "-Tsvg" });
        return s;
    }
    
}
