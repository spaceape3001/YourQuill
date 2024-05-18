////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/category/CategoryData.hpp>
#include <0/io/StdFile.hpp>

namespace yq::mithril {
    class Category::File : public StdFile, public Data {
    public:

        virtual bool    recursive_attributes() const override { return false; }
        virtual bool    has_body() const override { return false; }

    protected:
        virtual void    reset() override;

        virtual std::error_code   read(KVTree&&, std::string_view fname) override;
        virtual std::error_code   write(KVTree&) const override;
    };
}
