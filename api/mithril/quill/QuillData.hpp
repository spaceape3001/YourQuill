////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/bit/Copyright.hpp>
#include <mithril/bit/Policy.hpp>
#include <mithril/enum/Vcs.hpp>
#include <yq-toolbox/container/EnumMap.hpp>
#include <yq-toolbox/container/Set.hpp>
#include <yq-toolbox/container/Vector.hpp>

namespace yq::mithril {
    struct Copyright;

    struct QuillData {
        struct RootDir;

        //! Abbreviation
        std::string             abbr;
        
        //! Author
        std::string             author;
        
        //! Other ports that can cover this workspace
        Set<uint16_t>           aux_ports;
        
        //! Cache DB location
        std::string             cache;
        
        //! Our copyright....
        Copyright               copyright;

        //! Copyright string
        std::string             home;
        
        //! INI file path
        std::string             ini;
        
        
        //! Name of the local user
        std::string             local_user;

        //! Log directory
        std::string             log_dir;
        
        //! Name for this project
        std::string             name;
        
        //! Port number to use
        uint16_t                port            = 0;
        
        //! Read timeout
        unsigned int            read_timeout    = 0;
        
        //! RootDir data directories
        Vector<RootDir>            root_dirs;

        //! Temporary directory is here
        std::string             temp_dir;
        
        //! RootDir templates
        Vector<RootDir>            templates;
        
        //! Number of threads to spawn...
        unsigned int            threads         = 0;
        
        void                    reset();
        
    };

    struct QuillData::RootDir {
        std::string     key;
        std::string     name;
        std::string     path;
        PolicyMap       policy;
        Vcs             vcs;
        std::string     color;
        std::string     icon;
        
        //  users directory.... (default will be ./.users)
        
        RootDir() = default;
        explicit RootDir(const std::string_view& p) : path(p) {}
        ~RootDir(){}
    };
}
