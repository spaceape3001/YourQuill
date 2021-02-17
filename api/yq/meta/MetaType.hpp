////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Meta.hpp"

class MetaType : public Meta {
public:

    static const Vector<const MetaType*>&  all();
    static const MetaType*          lookup(unsigned int);
    static const MetaType*          lookup(const String&);

    virtual const char* generic() const override { return "MetaType"; }
    const char*                     file() const { return m_file; }

protected:
    MetaType(const char*, const char*, unsigned int);
    ~MetaType();
    
    virtual void                insert();
private:
    const char*                     m_file;
};
