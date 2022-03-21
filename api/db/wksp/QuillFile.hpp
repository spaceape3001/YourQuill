////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "QuillData.hpp"
#include <yq/file/AbstractFile.hpp>

namespace yq {

    class QuillFile : public AbstractFile, public QuillData {
    public:

        //struct RootInfo;

    protected:
        virtual void    reset() override;
        virtual bool    read(ByteArray&&buffer, const std::string_view& fname) override;
        virtual bool    write(yq::Stream&) const override;
    };

}
