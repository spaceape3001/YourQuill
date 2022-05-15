////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/db/StdFile.hpp>
#include <yq/wksp/QuillData.hpp>

namespace yq {

    class QuillFile : public StdFile, public QuillData {
    public:

        //struct RootInfo;

    protected:
        virtual void    reset() override;
        virtual bool        read(KVTree&&, std::string_view fname) override;
        virtual bool        write(KVTree&) const override;
    };

}