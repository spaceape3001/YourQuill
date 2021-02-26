////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Copyright.hpp"
#include "Policy.hpp"
#include "Vcs.hpp"

struct Copyright;

struct QuillData {
    struct RootData;

    //! Abbreviation
    String                  abbr;
    
    //! Author
    String                  author;
    
    //! Other ports that can cover this workspace
    Set<uint16_t>           aux_ports;
    
    //! Cache DB location
    String                  cache;
    
    //! Our copyright....
    Copyright               copyright;

    //! Copyright string
    String                  home;
    
    //! INI file path
    String                  ini;
    
    
    //! Name of the local user
    String                  local_user;

    //! Log directory
    String                  log_dir;
    
    //! Name for this project
    String                  name;
    
    //! Port number to use
    uint16_t                port;
    
    //! Read timeout
    unsigned int            read_timeout;
    
    //! Root data directories
    Vector<RootData>        roots;

    //! Temporary directory is here
    String                  temp_dir;
    
    //! Root templates
    Vector<RootData>        templates;
    
    void                    reset();
    
};

struct QuillData::RootData {
    String          key;
    String          name;
    String          path;
    PolicyMap       policy;
    Vcs             vcs;
    String          color;
    
    RootData(){}
    explicit RootData(const String&);
    ~RootData(){}
};
