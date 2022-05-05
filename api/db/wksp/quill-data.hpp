////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/bit/copyright.hpp>
#include <db/bit/policy.hpp>
#include <yq/enum/vcs.hpp>
#include <yq/collection/EnumMap.hpp>
#include <yq/collection/Set.hpp>
#include <yq/collection/Vector.hpp>

namespace yq {

    struct Copyright;

    struct QuillData {
        struct Root;

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
        
        //! Root data directories
        Vector<Root>            roots;

        //! Temporary directory is here
        std::string             temp_dir;
        
        //! Root templates
        Vector<Root>            templates;
        
        void                    reset();
        
    };

    struct QuillData::Root {
        std::string     key;
        std::string     name;
        std::string     path;
        PolicyMap       policy;
        Vcs             vcs;
        std::string     color;
        std::string     icon;
        
        //  users directory.... (default will be ./.users)
        
        Root() = default;
        explicit Root(const std::string_view& p) : path(p) {}
        ~Root(){}
    };
}
