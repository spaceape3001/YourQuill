////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Config.hpp"
#include <basic/BasicApp.hpp>

Config::Config()
{
    std::filesystem::path   hdir    = yq::BasicApp::user_home();
    if(hdir.empty())
        return ;
    
    configDir       = hdir / ".yquill";
    configFile      = configDir / "config";
    historyFile     = configDir / "history";
}

Config::~Config()
{
}
