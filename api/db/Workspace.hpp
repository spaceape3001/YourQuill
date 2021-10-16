////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/bit/Copyright.hpp>
#include <db/bit/Policy.hpp>

#include <db/enum/Access.hpp>
#include <db/enum/DataRole.hpp>
#include <db/enum/Vcs.hpp>

#include <util/EnumMap.hpp>
#include <util/SetFwd.hpp>

#include <filesystem>
#include <stdint.h>
#include <string_view>

class String;
struct Copyright;



struct Workspace {

    using fspath = std::filesystem::path;

    static constexpr const char*    szQuillFile = ".yquill";

    enum {
        SEARCH          = 0x1,  //  Search upward for quill fragment
        TEMPLATES_RO    = 0x2,  //  Templates are read only
        INIT_LOG        = 0x4
    };

    struct Root;        // will be replaced by the other one
    using RootVec       = Vector<const Root*>;
    using RootMap       = Map<String,const Root*, IgCase>;
    using RoleMap       = EnumMap<DataRole, const Root*>;
    using RoleVecMap    = EnumMap<DataRole, RootVec>;
    

    String              abbr;           //!< Abbrievation of the workspace
    String              author;         //!< Author of the workspace
    Set<uint16_t>       aux;            //!< Auxilary ports
    StringSet           available;      //!< Available templates
    fspath              cache;          //!< Cache DB location
    Copyright           copyright;      //!< Copyright information
    fspath              dot;            //!< DOT executable location
    fspath              git;            //!< GIT file location
    String              host;           //!< This hostname
    fspath              ini;            //!< INI location
    fspath              logs;           //!< Log files
    String              luser;          //!< Local user
    fspath              markdown;       //!< Location of the markdown scrtip
    String              name;           //!< Workspace name
    fspath              perl;           //!< Perl executable location
    fspath              pid;            //!< PID File location
    uint16_t            port = 0;       //!< Port number
    fspath              qdir;           //!< Total directory for th equill file
    fspath              qfile;          //!< Quill path file (full)
    String              qkey;           //!< Workspace's Quill Key (directory it's contained in
    fspath              qspec;          //!< The provided argument for the quill
    RootVec             roots;          //!< ALL roots active in this wokrspace
    RoleMap             rfirst;         //!< First-write roots (will be the first item in rwrite
    RootMap             rkey;           //!< Root by key
    RoleVecMap          rread;          //!< Root read order based on the data role
    unsigned int        rtimeout = 0;   //!< Read timeout on HTTP server
    RoleVecMap          rwrite;         //!< Root write order based on the data role
    fspath              smartypants;    //!< Smarthpants script location
    fspath              subversion;     //!< Subversion executable location
    StringSet           templates;      //!< Templates in use
    fspath              tmp;            //!< Temporary directory
    
    fspath              resolve(const fspath&) const;
    bool                load(const fspath&, unsigned opts = SEARCH|INIT_LOG);
    static bool         init(const fspath&, unsigned opts = SEARCH|INIT_LOG);
    
    /*! \brief Best guess of a quill file for the given path
    */
    static std::filesystem::path       best_guess(const std::filesystem::path&);
};

struct Workspace::Root {
    fspath                      path;
    EnumMap<DataRole,Access>    access;
    String                      key;
    String                      name, color,icon;
    uint16_t                    id  = 0;
    bool                        is_template = false;
};


extern const Workspace&        gWksp;
