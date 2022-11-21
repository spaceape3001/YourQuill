////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/StdFile.hpp>
#include <kernel/UserData.hpp>

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
        virtual bool        read(KVTree&&, std::string_view body, std::string_view fname) override;
        
        //! Override to write with body
        virtual bool        write(KVTree&, Stream&) const override;
    };
}
