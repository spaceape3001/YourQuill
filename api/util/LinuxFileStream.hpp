#pragma once

#include "Stream.hpp"
#include <filesystem>

class LinuxFileStream : public Stream {
public:
    enum {

        //  Causes the filestream to not use a temporary file (*.tmp)
        OVERWRITE       = 1,
        
        //  Does not BACKUP the file (timestamp)
        NO_BACKUP       = 2
    };

    LinuxFileStream();
    ~LinuxFileStream();
    
    bool            open(const std::filesystem::path&, unsigned int options=0);
    const std::filesystem::path&    path() const { return m_path; }
    bool            is_open() const { return m_fd != -1; }
    
    virtual bool    write(const char*z, size_t cb) override;
    virtual void    flush() override;

    void            close();
    

private:
    std::filesystem::path       m_path, m_tmp;
    
    //  there's already a cache in the file write mechanism
    //static constexpr const size_t   nBuffer = 65536;
    //char                        m_buffer[nBuffer];
    //size_t                      m_index = 0;
    
    int                         m_fd    = -1;
    unsigned int                m_options = 0;
    
    void                        do_close();
    void                        do_flush();
    
};
