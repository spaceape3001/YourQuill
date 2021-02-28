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

    struct RootInfo;

protected:
    virtual void    reset() override;
    virtual bool    read(Vector<char>&buffer, const std::string& fname) override;
    virtual bool    write(Vector<char>&) override;
};

