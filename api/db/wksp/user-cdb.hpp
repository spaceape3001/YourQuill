////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/db/CacheFwd.hpp>
#include <yq/enum/Sorted.hpp>
#include <yq/file/Document.hpp>
#include <yq/image/Image.hpp>
#include <yq/wksp/UserFile.hpp>

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
        using UserFragDoc  = std::pair<Fragment, User::SharedFile>;

        Vector<User>            all_users(Sorted sorted=Sorted());
        size_t                  all_users_count();
        
        bool                    any_users();
        
        std::string             brief(User);
        
        User                    db_user(Document, bool* wasCreated=nullptr);
        User                    db_user(Fragment, bool* wasCreated=nullptr);
        User                    db_user(std::string_view,  bool* wasCreated=nullptr);
        
        Document                document(User);
    
        //! Removes the user from the cache
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
        std::string             label(User);
        User                    make_user(std::string_view, const Root* rt=nullptr, cdb_options_t opts=0);
        User::SharedData        merged(User, cdb_options_t opts=0);
        
        std::string             name(User);
        
        Vector<UserFragDoc>     reads(User, cdb_options_t opts=0);
        Vector<UserFragDoc>     reads(User, class Root*, cdb_options_t opts=0);

        User::SharedData        update(User, cdb_options_t opts=0);
        void                    update_icon(User);

        User                    user(std::string_view);
        User                    user(Document, bool calc=false);
        User                    user(uint64_t);
        
        User::SharedFile        user_doc(Fragment, cdb_options_t opts=0);


        //!  \brief   Returns a writable document
        //!
        //!     \note the path will be CREATED by this method.
        //!
        //!     If the document already exists, it will be read in.
        User::SharedFile        write(User, const Root*, cdb_options_t opts=0);
    }
}
