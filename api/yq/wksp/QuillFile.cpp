////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "QuillFile.hpp"
#include "yq/bit/Copyright.hpp"

String              QuillFile::abbreviation() const
{
    return value(zAbbr);
}

String              QuillFile::author() const
{
    return value(zAuthor);
}

Set<uint16_t>       QuillFile::aux_ports() const
{
    Set<uint16_t>   ret;
    for(String & s : values(zAux)){
        uint16_r    r       = s.to_uint16();
        if(r.good)
            ret << r.value;
    }
    return ret;
}

String              QuillFile::cache() const
{
    return value(zCache);
}

Copyright           QuillFile::copyright() const
{
    Copyright   ret;
    const Attribute   *a  = nullptr;;
    if((a = first(zDisclaimer))){
        ret.stance  = AssertDeny::Deny;
    } else if((a = first(zNotice))){
        ret.stance  = AssertDeny::Neither;
    } else if((a = first(zCopyright))){
        ret.stance  = AssertDeny::Assert;
    }
    if(a){
        ret.text    = a->data;
        ret.from    = a->value(zFrom).to_ushort().value;
        ret.to      = a->value(zTo).to_ushort().value;
    }
    return ret;
}

String              QuillFile::home() const
{
    return value(zHome);
}

String              QuillFile::ini() const
{
    return value(zIni);
}

String              QuillFile::local_user() const
{
    return value(zLocalUser);
}

String              QuillFile::log_dir() const
{
    return value(zLogDir);
}

String              QuillFile::name() const
{
    return value(zName);
}

uint16_r            QuillFile::port() const
{
    return value(zPort).to_ushort();
}

unsigned_r          QuillFile::read_timeout() const
{
    return value(zReadTimeout).to_uinteger();
}


Vector<QuillFile::RootInfo>    QuillFile::roots() const
{
    Vector<RootInfo> ret;
    for(const Attribute* a : all(zRoot)){
        if(a->data.empty())
            continue;
        ret << RootInfo(a);
    }
    return ret;
}


String              QuillFile::temp_dir() const
{
    return value(zTempDir);
}


//! List of decalred templates
Vector<QuillFile::RootInfo>    QuillFile::templates() const
{
    Vector<RootInfo> ret;
    for(const Attribute* a : all(zTemplate)){
        if(a->data.empty())
            continue;
        ret << RootInfo(a);
    }
    return ret;
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

QuillFile::RootInfo::RootInfo() = default;

QuillFile::RootInfo::RootInfo(const String& p) : path(p)
{
}

QuillFile::RootInfo::~RootInfo() = default;

QuillFile::RootInfo::RootInfo(const Attribute*a)
{
    assert(a);
    if(!a)
        return ;

    path    = a->data;
    key     = a->value(zKey);
    color   = a->value(zColor);
    vcs     = Vcs(a->value(zVcs));
    name    = a->value(zName);
    for(DataRole dr : DataRole::all_values())
        policy[dr]  = Access(a->value(dr.key()));
}
