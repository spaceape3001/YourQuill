////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <ctime>
#include <string>
#include <basic/VersionSpec.hpp>

struct ConfigParams {
    //! Time session's considered "stale"
    time_t              autoLogout      = 900;
    
    //! Scan every five seconds for "stale" sessions
    time_t              staleScan       = 5;
    
    //! Require a new login every.... so often
    time_t              forceLogin      = 3600;
    
    //! Renew the cookie (&magic) this time-interval
    time_t              renewCookie     = 300;
    
    //! When to purge sessions
    time_t              idlePrune       = 450;
    
    //! How often to prune the table
    time_t              pruneInterval   = 60;
    
    //! Old magic is tolerated for this long
    time_t              oldMagic        = 5;
    
    //! Our cookie name
    std::string         cookieName      = "yq";

    //! Our version specification
    yq::VersionSpec     version         = yq::http10();
    
    //! Size of the SSID key
    unsigned int        sizeSSID        = 12;
    
    //! Size of the magic string
    unsigned int        sizeMAGIC       = 32;
    
        // max Rx body before complaining
    size_t              maxRxBody       = 6 << 20ULL;
    
    ConfigParams();
    ~ConfigParams();
};


extern const ConfigParams&    config;
