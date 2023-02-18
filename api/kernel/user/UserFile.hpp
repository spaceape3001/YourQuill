////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/io/StdFile.hpp>
#include <kernel/user/UserData.hpp>

namespace yq {
    class User::File : public StdFile, public User::Data {
    public:
    
        //  Set this before load
        bool            skip_bio    = false;
    
        virtual bool    recursive_attributes() const override { return true; }
        virtual bool    has_body() const override { return true; }

    protected:
        virtual void    reset() override;

        //! Override to read with body
        virtual std::error_code         read(KVTree&&, std::string_view body, std::string_view fname) override;
        
        //! Override to write with body
        virtual std::error_code         write(KVTree&, Stream&) const override;
    };
}
