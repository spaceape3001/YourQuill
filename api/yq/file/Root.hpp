////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RootFwd.hpp"
#include <yq/bit/Policy.hpp>
#include <yq/collection/EnumMap.hpp>
#include <yq/enum/Access.hpp>
#include <yq/enum/Vcs.hpp>
#include <yq/type/Flag.hpp>


namespace yq {

    namespace wksp { struct Impl; }

    /*! \brief Represents a "root" directory for the workspace, all keys are relative to one of these.
    */
    struct Root {
        friend struct wksp::Impl;
        
        EnumMap<DataRole, Access>   access;
        std::string                 color;
        uint64_t                    depth           = 0;    // used by the workspace during load to shuffle into order
        std::string                 icon;               // file
        uint64_t                    id              = 0;
        bool                        is_template     = false;
        std::string                 key;
        std::string                 name;
        const std::filesystem::path path;
        Flag<Vcs>                   vcs;
        

        static const Root*  by_key(const std::string_view&);

        bool                has_git() const;
        bool                has_subversion() const;

        //  version control
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

        std::filesystem::path   resolve(const std::filesystem::path&) const;
        
        
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
