////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/core/std_file.hpp>
#include <db/docsys/leaf-data.hpp>

namespace yq {

    //! Leaf, the basis of all evil :)
    class Leaf::File : public StdFile, public Data {
    public:

        virtual void    reset() override;
        virtual bool    read(KVTree&&, std::string_view) override;
        virtual bool    write(Stream&) const override;
        virtual bool    recursive_attributes() const { return true; }
        virtual bool    has_body() const { return true; }
    };
}
