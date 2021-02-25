////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <QString>


class FileName {
public:

    FileName();
    FileName(const QString&);
    FileName(const FileName&);
    FileName(FileName&&);
    ~FileName();
    FileName&           operator=(const FileName&);
    FileName&           operator=(FileName&&);
    
    FileName&           operator=(const QString&);
    
    const QString&      full() const { return m_full; }
    const QString&      name() const { return m_name; }
    const QString&      path() const { return m_path; }
    
    bool                isEmpty() const { return m_full.isEmpty(); }
    void                clear();
    
private:
    void    _set(const QString&);
    QString m_full, m_name, m_path;
};
