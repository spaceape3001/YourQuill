////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <filesystem>
#include <vector>

struct History {
    std::vector<std::filesystem::path>       paths;

    History();
    ~History();
    
    bool    has(const std::filesystem::path& fp) const;
    void    limit(unsigned mx);
    void    erase(const std::filesystem::path& fp);
    void    load(const std::filesystem::path& fp);
    void    save(const std::filesystem::path&fp) const;
};
