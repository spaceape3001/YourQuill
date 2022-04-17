////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "PidFile.hpp"
#include <yq/file/file_utils.hpp>
#include <fstream>

namespace yq {
    PidFile::PidFile(const std::filesystem::path& f, bool autoMake)
    {
        m_file  = f;
        m_first = !file_exists(m_file.c_str());
        if(m_first && autoMake)
            make();
        
    }

    void     PidFile::make()
    {
        std::ofstream   p(m_file);
        p << getpid();
    }

    PidFile::~PidFile()
    {
        if(m_first){
            remove(m_file.c_str());
        }
    }
}
