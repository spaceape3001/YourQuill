////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/preamble.hpp>
#include <yq/c++/NoCopy.hpp>
#include <yq/c++/NoMove.hpp>

namespace yq {

    class PidFile : NoCopy, NoMove {
    public:

        PidFile(const std::filesystem::path&, bool autoMake=true);
        ~PidFile();
        bool            first() const { return m_first; }
        void            make();
        const std::filesystem::path&    file() const { return m_file; }
        
    private:
        std::filesystem::path   m_file;
        bool                    m_first;
    };
}
