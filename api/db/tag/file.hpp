////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "data.hpp"
#include <yq/file/XmlFile.hpp>

namespace yq {

    class Tag::File : public XmlFile, public Data {
    public:

    protected:
        virtual void    reset() override;
        virtual bool    read(ByteArray&&buffer, const std::string_view& fname) override;
        virtual bool    read(const XmlDocument&, std::string_view fname) override;
        virtual bool    write(XmlDocument&) const override;
    };
}
