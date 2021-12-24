////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "QuillData.hpp"
#include "StdFile.hpp"


class QuillFile : public AbstractFile, public QuillData {
public:

    //struct RootInfo;

protected:
    virtual void    reset() override;
    virtual bool    read(ByteArray&&buffer, const std::string& fname) override;
    virtual bool    write(Stream&) const override;
};

