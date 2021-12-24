#pragma once

#include "Stream.hpp"
#include <filesystem>
#include <fstream>

class FileStream : public Stream {
public:

    FileStream();
    ~FileStream();
    
    bool        open(const std::filesystem::path&);
    void        close();
    bool        is_open() const;
    
    virtual bool    write(const char*z, size_t cb) override;
    virtual void    flush() override;
    
    const std::filesystem::path&   path() const { return m_path; }
    
private:
    std::ofstream           m_file;
    std::filesystem::path   m_path;
};


