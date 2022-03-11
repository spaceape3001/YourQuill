////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <util/NoCopy.hpp>
#include <util/NoMove.hpp>
#include <util/String.hpp>

class PidFile : NoCopy, NoMove {
public:

    PidFile(const String&, bool autoMake=true);
    ~PidFile();
    bool            first() const { return m_first; }
    void            make();
    
private:
    String          m_file;
    bool            m_first;
};
