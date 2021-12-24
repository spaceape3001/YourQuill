////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ClassData.hpp"
#include "StdFile.hpp"

#include <util/AbstractFile.hpp>
#include <util/Set.hpp>

//! Atom classes (including fields)
class ClassFile : public AbstractFile, public ClassData {
public:

    using Shared    = std::shared_ptr<ClassFile>;

protected:
    virtual void    reset() override;
    virtual bool    read(ByteArray&&buffer, const std::string& fname) override;
    virtual bool    write(Stream&) const override;
};

