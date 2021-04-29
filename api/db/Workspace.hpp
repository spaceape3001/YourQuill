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

#include <stdint.h>
#include <string_view>

class String;
struct Copyright;



struct Workspace {

    struct Root;
    
    using RootVec      = Vector<Root*>;
    using RootMap      = EnumMap<DataRole, Root*>;
    using RootMapVec   = EnumMap<DataRole,RootVec>;

    enum {
        SEARCH          = 0x1,  //  Search upward for quill fragment
        TEMPLATES_RO    = 0x2,  //  Templates are read only
        INIT_LOG        = 0x4
    };

    String              author;
    String              abbr;
    Set<uint16_t>       aux;
    String              cache;      // default (for blank tmp) is /tmp/yquill/cache/${qkey}.db
    Copyright           copyright;
    String              ini;        // default (for blank tmp) is /tmp/yquill/ini/${qkey}.ini
    String              logs;       // default (for blank tmp) is /tmp/yquill/logs/${qkey}-${tmp}.log
    String              luser;
    String              name;
    String              pid;
    uint16_t            port        = 0;
    String              qkey;
    String              qfile;
    String              qdir;
    StringSet           templates;
    RootVec             roots;
    RootMap             rfirst;
    RootMapVec          rread;
    RootMapVec          rwrite;
    String              tmp;        // will not override IPC & PIDs (which remain in /tmp/yquill), but will override cache & logs
    
    bool                init(const std::string_view&, unsigned opts = SEARCH|INIT_LOG);
    String              resolve(const std::string_view&) const;
    
    static String       dot;
    static String       git;
    static String       hostname;
    static String       markdown;
    static String       smartypants;
    static String       perl;
    static String       subversion;
    static StringSet    available;  // templates

private:    
    static bool         sinit();
};

struct Workspace::Root {
    String                      path;
    EnumMap<DataRole,Access>    access;
    String                      key;
    String                      name, color,icon;
    uint16_t                    id  = 0;
    bool                        is_template = false;
    
};


extern Workspace        gWksp;
