////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "TagData.hpp"
#include <util/AbstractFile.hpp>

class TagFile : public AbstractFile, public TagData {
public:

    using Shared        = std::shared_ptr<TagFile>;

protected:
    virtual void    reset() override;
    virtual bool    read(Vector<char>&buffer, const std::string& fname) override;
    virtual bool    write(Vector<char>&) override;
};
