////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "tag_data.hpp"
#include <yq/file/XmlFile.hpp>

namespace yq {

    class Tag::File : public AbstractFile, public Data {
    public:

    protected:
        virtual void    reset() override;
        virtual bool    read(ByteArray&&buffer, const std::string_view& fname) override;
        virtual bool    write(yq::Stream&) const override;
    };
}
