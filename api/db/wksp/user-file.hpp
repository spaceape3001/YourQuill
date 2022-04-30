////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/wksp/user-data.hpp>
#include <db/core/std_file.hpp>

namespace yq {
    class User::File : public StdFile, public User::Data {
    public:
    
        //  Set this before load
        bool            skip_bio    = false;
    
    protected:
        virtual void    reset() override;

        //! Override to read with body
        virtual bool        read(KVTree&&, std::string_view body, std::string_view fname) override;
        
        //! Override to write with body
        virtual bool        write(KVTree&, Stream&) const override;
    };
}
