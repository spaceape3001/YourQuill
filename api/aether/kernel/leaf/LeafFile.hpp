////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <aether/kernel/io/StdFile.hpp>
#include <aether/kernel/leaf/LeafData.hpp>

namespace yq {

    //! Leaf, the basis of all evil :)
    class Leaf::File : public StdFile, public Data {
    public:

        virtual void    reset() override;
        virtual std::error_code   read(KVTree&&, std::string_view, std::string_view fname) override;
        virtual std::error_code   write(Stream&) const override;
        virtual bool    recursive_attributes() const { return true; }
        virtual bool    has_body() const { return true; }
    };
}
