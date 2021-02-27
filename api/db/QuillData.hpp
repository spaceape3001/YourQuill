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
    struct Root;

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
    Vector<Root>            roots;

    //! Temporary directory is here
    String                  temp_dir;
    
    //! Root templates
    Vector<Root>            templates;
    
    void                    reset();
    
};

struct QuillData::Root {
    String          key;
    String          name;
    String          path;
    PolicyMap       policy;
    Vcs             vcs;
    String          color;
    
    Root(){}
    explicit Root(const String& p) : path(p) {}
    ~Root(){}
};
