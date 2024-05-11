////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#error "Do NOT include (for now)"

#include <mithril/io/StdFile.hpp>
#include <mithril/novel/NovelData.hpp>

namespace yq::mithril {

    //! Novel, the basis of all evil :)
    class Novel::File : /* public StdFile,*/  public Data {
    public:

        virtual void    reset();
        
    /* 
        virtual std::error_code   read(KVTree&&, std::string_view, std::string_view fname) override;
        virtual std::error_code   write(Stream&) const override;
        virtual bool    recursive_attributes() const { return true; }
        virtual bool    has_body() const { return true; }
    */
    };
}
