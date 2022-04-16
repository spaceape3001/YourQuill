////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "User.hpp"
#include "UserFile.hpp"
#include "UserGroup.hpp"
#include "UserGroupFile.hpp"
#include <yq/enum/Sorted.hpp>
#include <yq/file/Document.hpp>
#include <yq/image/Image.hpp>

namespace yq {
    struct Fragment;
    struct Root;

    struct User::Info {
        Document        doc;
        std::string     key;
        Image           icon;
        std::string     name;
        bool operator==(const Info&) const = default;
    };

    struct UserGroup::Info {
        Document        doc;
        std::string     key;
        Image           icon;
        std::string     name;
        bool operator==(const Info&) const = default;
    };

    using UserFragDoc       = std::pair<Fragment, User::SharedFile>;
    using UserGroupFragDoc  = std::pair<Fragment, UserGroup::SharedFile>;

    namespace cdb {
        struct NKI;
        
        enum {
            SKIP_BIO        = 0x100
        };
        

        std::vector<UserGroup>  all_usergroups(Sorted sorted=Sorted());
        size_t                  all_usergroups_count();
        
        std::vector<User>       all_users(Sorted sorted=Sorted());
        size_t                  all_users_count();
        
        User                    db_user(Document, bool* wasCreated=nullptr);
        User                    db_user(Fragment, bool* wasCreated=nullptr);
        User                    db_user(std::string_view,  bool* wasCreated=nullptr);
        
        UserGroup               db_usergroup(Document, bool* wasCreated=nullptr);
        UserGroup               db_usergroup(Fragment, bool* wasCreated=nullptr);
        UserGroup               db_usergroup(std::string_view,  bool* wasCreated=nullptr);

        Document                document(User);
        Document                document(UserGroup);
    
        void                    erase(User);
        void                    erase(UserGroup);
        bool                    exists(User);
        bool                    exists(UserGroup);

        bool                    exists_user(uint64_t);
        bool                    exists_usergroup(uint64_t);
        Image                   icon(User);
        Image                   icon(UserGroup);
        User::Info              info(User, bool autoKeyToName=false);
        UserGroup::Info         info(UserGroup, bool autoKeyToName=false);
        std::string             key(User);
        std::string             key(UserGroup);
        User                    make_user(std::string_view, const Root* rt=nullptr);
        UserGroup               make_usergroup(std::string_view, const Root* rt=nullptr);
        User::SharedData        merged(User, unsigned int opts=0);
        UserGroup::SharedData   merged(UserGroup, unsigned int opts=0);
        
        std::vector<UserFragDoc> reads(User);
        std::vector<UserFragDoc> reads(User, class Root*);
        std::vector<UserGroupFragDoc> reads(UserGroup);
        std::vector<UserGroupFragDoc> reads(UserGroup, class Root*);

        User                    user(std::string_view);
        User                    user(uint64_t);
        
        User::SharedFile        user_doc(Fragment, bool fAllowEmpty=false);

        UserGroup               usergroup(std::string_view);
        UserGroup               usergroup(uint64_t);

        UserGroup::SharedFile   usergroup_doc(Fragment, bool fAllowEmpty=false);

        //!  \brief   Returns a writable document
        //!
        //!     \note the path will be CREATED by this method.
        //!
        //!     If the document already exists, it will be read in.
        User::SharedFile        write(User, const Root*);

        UserGroup::SharedFile   write(UserGroup, const Root*);
    }
}
