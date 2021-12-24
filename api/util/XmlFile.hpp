////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "AbstractFile.hpp"
#include "Enum.hpp"
#include "XmlFwd.hpp"
#include <initializer_list>

class XmlFile : public AbstractFile {
public:

protected:
    XmlFile();
    ~XmlFile();

    virtual bool    read(const XmlDocument&, const std::string& fname) = 0;
    virtual bool    write(XmlDocument&) const = 0;

    virtual bool    read(ByteArray&&, const std::string& fname) override;
    virtual bool    write(yq::Stream&) const override;

    virtual bool    is_binary() const override { return false; }
};
