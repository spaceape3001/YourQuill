////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ClassData.hpp"

#include <yq/file/AbstractFile.hpp>
#include <yq/collection/Set.hpp>

namespace yq {

    //! Atom classes (including fields)
    class ClassFile : public AbstractFile, public ClassData {
    public:

        using Shared    = std::shared_ptr<ClassFile>;

    protected:
        virtual void    reset() override;
        virtual bool    read(ByteArray&&buffer, const std::string_view& fname) override;
        virtual bool    write(yq::Stream&) const override;
    };

}
