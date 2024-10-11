////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RootFwd.hpp"
#include <mithril/bit/Policy.hpp>
#include <mithril/enum/Access.hpp>
#include <mithril/enum/Vcs.hpp>
#include <yq/container/EnumMap.hpp>
#include <yq/core/Flag.hpp>


namespace yq::mithril {
    namespace wksp { struct Impl; }

    /*! \brief Represents a "root_dir" directory for the workspace, all keys are relative to one of these.
    */
    struct RootDir {
        friend struct wksp::Impl;
        
        //! Access policy for this root_dir
        EnumMap<DataRole, Access>   access;
        
        //! Suggestive color for this root_dir
        std::string                 color;
        
        //! Used by the workspace during load to shuffle into order
        uint64_t                    depth           = 0;  
        
        //! Filename of the icon for this root_dir
        std::string                 icon;               // file
        
        //! ID for this root_dir
        uint64_t                    id              = 0;
        
        //! TRUE if this was loaded as a template
        bool                        is_template     = false;
        
        //! Short key for this root_dir (user defined)
        std::string                 key;
        
        //! Name of this root_dir
        std::string                 name;
        
        //! Path to the root_dir (assume case-sensitive)
        const std::filesystem::path path;
        
        //! Detected version control systems active for the root_dir
        Flag<Vcs>                   vcs;
        
        //! Look up the root_dir by key
        static const RootDir*  by_key(const std::string_view&);

        //! Look up the root_dir by key
        static const RootDir*  by_id(uint64_t);

        //! TRUE if this root_dir is controlled by GIT
        bool                has_git() const;
        
        //! TRUE if this root_dir is controlled by subverision
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

        //! Resolves a relative file to this root_dir (file existence not checked)
        std::filesystem::path   resolve(const std::filesystem::path&) const;
        
        //! Returns the policy (from the map) for the particular data role
        Access              policy(DataRole) const;
        
    private:

        RootDir(const std::filesystem::path&);
        RootDir(const std::filesystem::path&, PolicyMap);


        RootDir(const RootDir&) = delete;
        RootDir(RootDir&&) = delete;
        
        ~RootDir();
        
        RootDir&    operator=(const RootDir&) = delete;
        RootDir&    operator=(RootDir&&) = delete;
        
    };
    
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    
    const RootDir*  root_dir(Root);
}
