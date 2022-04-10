////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <yq/file/AbstractFile.hpp>
#include <yq/type/Enum.hpp>
#include <yq/file/XmlFwd.hpp>
#include <initializer_list>

namespace yq {

    class XmlFile : public AbstractFile {
    public:

    protected:
        XmlFile();
        ~XmlFile();

        virtual bool    read(const XmlDocument&, const std::string_view& fname) = 0;
        virtual bool    write(XmlDocument&) const = 0;

        virtual bool    read(ByteArray&&, const std::string_view& fname) override;
        virtual bool    write(yq::Stream&) const override;

        virtual bool    is_binary() const override { return false; }
    };
    
}
