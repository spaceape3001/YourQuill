////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "cdb_common.hpp"
#include "user_file.hpp"
#include "document.hpp"
#include "image.hpp"

#include <yq/enum/Sorted.hpp>

namespace yq {
    struct Fragment;
    struct Root;

    struct User::Info {
        Document        doc;
        std::string     key;
        Image           icon;
        std::string     name;
        std::string     brief;
        bool            is_owner    = false;
        bool            is_admin    = false;
        bool            is_writer   = false;
        bool            is_reader   = false;
        bool operator==(const Info&) const = default;
    };



    namespace cdb {
        enum {
            SKIP_BIO        = 0x100
        };
        using UserFragDoc  = std::pair<Fragment, User::SharedFile>;

        Vector<User>            all_users(Sorted sorted=Sorted());
        size_t                  all_users_count();
        
        bool                    any_users();
        
        std::string             brief(User);
        
        User                    db_user(Document, bool* wasCreated=nullptr);
        User                    db_user(Fragment, bool* wasCreated=nullptr);
        User                    db_user(std::string_view,  bool* wasCreated=nullptr);
        
        Document                document(User);
    
        void                    erase(User);
        bool                    exists(User);

        bool                    exists_user(uint64_t);
        Image                   icon(User);
        User::Info              info(User, bool autoKeyToName=false);
        
        bool                    is_owner(User);
        bool                    is_admin(User);
        bool                    is_writer(User);
        bool                    is_reader(User);
        
        std::string             key(User);
        User                    make_user(std::string_view, const Root* rt=nullptr);
        User::SharedData        merged(User, unsigned int opts=0);
        
        std::string             name(User);
        
        Vector<UserFragDoc>     reads(User);
        Vector<UserFragDoc>     reads(User, class Root*);

        User                    user(std::string_view);
        User                    user(uint64_t);
        
        User::SharedFile        user_doc(Fragment, bool fAllowEmpty=false);


        //!  \brief   Returns a writable document
        //!
        //!     \note the path will be CREATED by this method.
        //!
        //!     If the document already exists, it will be read in.
        User::SharedFile        write(User, const Root*);
    }
}
