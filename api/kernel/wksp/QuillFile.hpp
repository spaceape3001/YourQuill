////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/io/StdFile.hpp>
#include <kernel/wksp/QuillData.hpp>

namespace yq {

    class QuillFile : public StdFile, public QuillData {
    public:

        //struct RootInfo;

    protected:
        virtual void    reset() override;
        virtual std::error_code     read(KVTree&&, std::string_view fname) override;
        virtual std::error_code     write(KVTree&) const override;
    };

}
