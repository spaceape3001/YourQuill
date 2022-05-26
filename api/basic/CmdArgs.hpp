////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <span>
#include <string_view>

namespace yq {

    class CmdArgs {
    public:
        
        CmdArgs(const CmdArgs&) = delete;
        CmdArgs(CmdArgs&&) = delete;
        CmdArgs& operator=(const CmdArgs&) = delete;
        CmdArgs& operator=(CmdArgs&&) = delete;
        
        CmdArgs(int, char**);
        ~CmdArgs();
        
        static std::string_view         appName();
        static std::span<const char*>   commandLine();

    private:
        struct Repo;
        static Repo& repo();
    };
}
