////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/orgsys/category-data.hpp>
#include <yq/db/StdFile.hpp>

namespace yq {

    class Category::File : public StdFile, public Data {
    public:

        virtual bool    recursive_attributes() const override { return false; }
        virtual bool    has_body() const override { return false; }

    protected:
        virtual void    reset() override;

        virtual bool    read(KVTree&&, std::string_view fname) override;
        virtual bool    write(KVTree&) const override;
    };
}
