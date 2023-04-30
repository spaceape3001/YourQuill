////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "History.hpp"
#include <fstream>

History::History()
{
}

History::~History()
{
}

bool    History::has(const std::filesystem::path& fp) const
{
    for(auto& p : paths)
        if(p == fp)
            return true;
    return false;
}

void History::limit(unsigned mx)
{
    if(paths.size() > mx){
        auto b  = paths.begin() + (paths.size() - mx);
        paths       = std::vector<std::filesystem::path>(b, paths.end());
    }
}

void    History::erase(const std::filesystem::path& fp)
{
    std::erase(paths, fp);
}

void    History::load(const std::filesystem::path& fp)
{
    if(!fp.empty()){
        static constexpr const size_t   N   = 1024;
        char    line[N];
        paths.clear();
        std::ifstream   fin(fp);
        while(fin){
            line[0] = '\0';
            fin.getline(line, N, '\n');
            if(line[0])
                paths.push_back(line);
        }
    }
}

void    History::save(const std::filesystem::path&fp) const
{
    if(!fp.empty()){
        std::ofstream fout(fp);
        for(auto&p : paths)
            fout << p << '\n';
    }
}
