////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/preamble.hpp>
#include <yq/mithril/user/UserFile.hpp>

#define TBL_USERS "Users"

namespace yq::mithril::cdb {

    /*! \brief List of all users
    
        \param[in] sorted   Set to sort by key
    */
    Vector<User>            all_users(Sorted sorted=Sorted());
    
    //! \brief Total number of users
    size_t                  all_users_count();
    
    //! \brief TRUE if there are *any* users
    bool                    any_users();
    
    //! \brief Brief description to the user
    std::string             brief(User);
    
    //! \brief Creates/Gets a user by document
    User                    db_user(Document, bool* wasCreated=nullptr);

    //! \brief Creates/Gets a user by fragment
    User                    db_user(Fragment, bool* wasCreated=nullptr);

    //! \brief Creates/Gets a user by key
    User                    db_user(std::string_view,  bool* wasCreated=nullptr);
    
    //! \brief Document for user
    Document                document(User);
    
    //! Checks for user existance (in cache database)
    bool                    exists(User);

    //! Checks for user existance (in cache database)
    bool                    exists_user(uint64_t);
    
    //! Icon for user
    Image                   icon(User);
    
    //! User information (one stop)
    User::Info              info(User, bool autoKeyToName=false);
    
    //! TRUE if user is marked as administrator
    bool                    is_admin(User);

    //! TRUE if user is marked as guest
    bool                    is_guest(User);

    //! TRUE if user is marked as owner
    bool                    is_owner(User);

    //! TRUE if user is marked as reader
    bool                    is_reader(User);

    //! TRUE if user is marked as writer
    bool                    is_writer(User);

    //! Key of user
    std::string             key(User);
    
    //! Label of user
    std::string             label(User);
    
    /*! \brief Makes a user
    
        \param[in] k            Key (ie username)
        \param[in] rt           RootDir to create it in, null goes for default
        \param[in] opts         options
        \param[in] wasCreated   returns TRUE if created
        
        \return Valid user if successful, empty otherwise
    */
    User                    make_user(std::string_view k, const RootDir* rt=nullptr, cdb_options_t opts=0, bool* wasCreated=nullptr);
    
    //! Merged user data
    User::SharedData        merged(User, cdb_options_t opts=0);
    
    //! Name of the user
    std::string             name(User);
    
    //! All user fragments
    Vector<UserFragDoc>     reads(User, cdb_options_t opts=0);
    //! User fragments under specified root_dir
    Vector<UserFragDoc>     reads(User, class RootDir*, cdb_options_t opts=0);

    //! Finds user with matching key
    User                    user(std::string_view);
    
    //! Finds user based on document
    User                    user(Document, bool calc=false);

    //! Finds user based on ID
    User                    user(uint64_t);
    
    //! User document for fragment
    User::SharedFile        user_doc(Fragment, cdb_options_t opts=0);


    //!  \brief   Returns a writable document
    //!
    //!     \note the path will be CREATED by this method.
    //!
    //!     If the document already exists, it will be read in.
    User::SharedFile        write(User, const RootDir*, cdb_options_t opts=0);
}
