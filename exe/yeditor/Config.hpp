////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <filesystem>

struct Config {
    std::filesystem::path       configDir;
    std::filesystem::path       configFile;
    std::filesystem::path       historyFile;
    unsigned int                historyLimit    = 10;

    
    Config();
    ~Config();
};
