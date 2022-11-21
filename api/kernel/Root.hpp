////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RootFwd.hpp"
#include <kernel/Policy.hpp>
#include <kernel/Access.hpp>
#include <kernel/Vcs.hpp>
#include <basic/EnumMap.hpp>
#include <basic/Flag.hpp>


namespace yq {

    namespace wksp { 
        struct Impl; 
    }

    /*! \brief Represents a "root" directory for the workspace, all keys are relative to one of these.
    */
    struct Root {
        friend struct wksp::Impl;
        
        //! Access policy for this root
        EnumMap<DataRole, Access>   access;
        
        //! Suggestive color for this root
        std::string                 color;
        
        //! Used by the workspace during load to shuffle into order
        uint64_t                    depth           = 0;  
        
        //! Filename of the icon for this root
        std::string                 icon;               // file
        
        //! ID for this root
        uint64_t                    id              = 0;
        
        //! TRUE if this was loaded as a template
        bool                        is_template     = false;
        
        //! Short key for this root (user defined)
        std::string                 key;
        
        //! Name of this root
        std::string                 name;
        
        //! Path to the root (assume case-sensitive)
        const std::filesystem::path path;
        
        //! Detected version control systems active for the root
        Flag<Vcs>                   vcs;
        
        //! Look up the root by key
        static const Root*  by_key(const std::string_view&);

        //! TRUE if this root is controlled by GIT
        bool                has_git() const;
        
        //! TRUE if this root is controlled by subverision
        bool                has_subversion() const;

        //  version control (NYI)
        void                move(const std::filesystem::path&, const std::filesystem::path&);
        void                remove(const std::filesystem::path&);
        void                add(const std::filesystem::path&);
        bool                commit();
        bool                update();


        //  From the quill-doc (relative)
        //const String&       def_icon_file() const { return m_icon; }
        
        bool                is_good(DataRole, Access) const;
        bool                is_readable(DataRole) const;
        bool                is_writable(DataRole) const;
        bool                is_write_first(DataRole) const;

        //! Resolves a relative file to this root (file existence not checked)
        std::filesystem::path   resolve(const std::filesystem::path&) const;
        
        //! Returns the policy (from the map) for the particular data role
        Access              policy(DataRole) const;
        
    private:

        Root(const std::filesystem::path&);
        Root(const std::filesystem::path&, PolicyMap);


        Root(const Root&) = delete;
        Root(Root&&) = delete;
        
        ~Root();
        
        Root&    operator=(const Root&) = delete;
        Root&    operator=(Root&&) = delete;
        
    };
}
