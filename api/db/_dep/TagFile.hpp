////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "TagData.hpp"
#include <yq/file/AbstractFile.hpp>

namespace yq {

    class TagFile : public AbstractFile, public TagData {
    public:

        using Shared        = std::shared_ptr<TagFile>;

    protected:
        virtual void    reset() override;
        virtual bool    read(ByteArray&&buffer, const std::string& fname) override;
        virtual bool    write(yq::Stream&) const override;
    };
}
