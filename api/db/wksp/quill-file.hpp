////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/core/std_file.hpp>
#include <db/wksp/quill-data.hpp>

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
