////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/keyv/StdFile.hpp>
#include <mithril/leaf/LeafData.hpp>

namespace yq::mithril {

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
