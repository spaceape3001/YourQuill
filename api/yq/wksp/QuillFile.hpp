////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "yq/bit/Policy.hpp"
#include "yq/enum/Vcs.hpp"
#include "yq/io/StdFile.hpp"

struct Copyright;

class QuillFile : public StdFile {
public:

    struct RootInfo;
    
    static constexpr const char*    szFile  = ".yquill";
    
    
    static constexpr Seq    zAbbr           = { "abbr", "abbreviation" };
    static constexpr Seq    zAuthor         = { "author" };
    static constexpr Seq    zAux            = { "aux" };
    static constexpr Seq    zCache          = { "cache" };
    static constexpr Seq    zColor          = { "color" };
    static constexpr Seq    zCopyright      = { "copyright" };
    static constexpr Seq    zDisclaimer     = { "disclaimer", "disclaim" };
    static constexpr Seq    zFrom           = { "from" };
    static constexpr Seq    zHome           = { "home" };
    static constexpr Seq    zKey            = { "key", "k" };
    static constexpr Seq    zIni            = { "ini" };
    static constexpr Seq    zLocalUser      = { "local", "user", "local_user" };
    static constexpr Seq    zLogDir         = { "logs" };
    static constexpr Seq    zName           = { "name" };
    static constexpr Seq    zNotice         = { "notice" };
    static constexpr Seq    zPort           = { "port" };
    static constexpr Seq    zReadTimeout    = { "timeout" };
    static constexpr Seq    zRoot           = { "root", "r" };
    static constexpr Seq    zTo             = { "to" };
    static constexpr Seq    zTempDir        = { "tmp", "temp", "temp_dir", "tmpdir" };
    static constexpr Seq    zTemplate       = { "template", "t" };
    static constexpr Seq    zVcs            = { "vcs" };

    //! Abbrievation for the workspace/project
    String              abbreviation() const;
    
    //! Auxillary ports are suitable alternative ports to check before invoking the server
    Set<uint16_t>       aux_ports() const;
    //void                aux_ports(const Set<uint16_t>&);
 
    //! Copyright declaration
    Copyright           copyright() const;
    
    //! Home-URL for the workspace
    String              home() const;
    
    //!  INI is the location for the INI that is required for the webserver.
    //!  It defaults to temp/ini
    String              ini() const;
    
    //! User naem for local access
    String              local_user() const;
    
    //! Location for logs, defaults to temp/logs
    String              log_dir() const;
    
    //! Name of the project/workspace
    String              name() const;
    
    //!  OUR port number (don't choose zero)
    uint16_r            port() const;
    
    //!  List of declared roots
    Vector<RootInfo>    roots() const;
    
    //!  Temp dir
    String              temp_dir() const;
    
    
    //! List of decalred templates
    Vector<RootInfo>    templates() const;

private:
    virtual bool        recursive_attributes() const override { return true; }
    virtual bool        has_body() const override { return false; }
};

struct QuillFile::RootInfo {
    String          key;
    String          name;
    String          path;
    PolicyMap       policy;
    Vcs             vcs;
    String          color;
    
    RootInfo();
    explicit RootInfo(const Attribute*);
    explicit RootInfo(const String&);
    ~RootInfo();
};
