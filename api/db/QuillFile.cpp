////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "QuillFile.hpp"
#include "util/Utilities.hpp"


void    QuillData::reset()
{
    abbr.clear();
    author.clear();
    aux_ports.clear();
    cache.clear();
    copyright.reset();
    home.clear();
    ini.clear();
    local_user.clear();
    log_dir.clear();
    name.clear();
    port    = 0;
    read_timeout    = 0;
    roots.clear();
    temp_dir.clear();
    templates.clear();
}

void    QuillFile::reset() 
{
    QuillData::reset();
}

namespace {
    QuillData::RootData    parse_root(const Attribute* a)
    {
        QuillData::RootData ret;
        ret.path    = a->data;
        ret.key     = a->value({ "key", "k" });
        ret.color   = a->value("color");
        ret.vcs     = Vcs(a->value("vcs"));
        ret.name    = a->value("name");
        for(DataRole dr : DataRole::all_values())
            ret.policy[dr]  = Access(a->value(dr.key()));
        return ret;
    }
}

bool    QuillFile::read(Vector<char>&buffer, const std::string& fname) 
{
    AttrTree        attrs;
    if(!attrs.parse(buffer, nullptr, true, fname))
        return false;
    
    abbr            = attrs.value({ "abbr", "abbreviation" });
    author          = attrs.value("author");
    aux_ports       = attrs.values_set_u16({"aux"});
    cache           = attrs.value("cache");
    
    const Attribute   *a  = nullptr;;
    if((a = attrs.first({"disclaimer", "disclaim"}))){
        copyright.stance  = AssertDeny::Deny;
    } else if((a = attrs.first("notice"))){
        copyright.stance  = AssertDeny::Neither;
    } else if((a = attrs.first("copyright"))){
        copyright.stance  = AssertDeny::Assert;
    }
    if(a){
        copyright.text    = a->data;
        copyright.from    = a->value("from").to_ushort().value;
        copyright.to      = a->value("to").to_ushort().value;
    }
    
    home            = attrs.value("home");
    ini             = attrs.value("ini");
    local_user      = attrs.value({"local", "user", "local_user"});
    log_dir         = attrs.value("logs");
    name            = attrs.value("name");
    port            = attrs.value("port").to_uint16().value;
    read_timeout    = attrs.value("timeout").to_uinteger().value;
    temp_dir        = attrs.value({"temp", "tmp", "tempdir", "temp_dir"});
    
    for(const Attribute* a : attrs.all({"root", "r"}))
        roots << parse_root(a);
    for(const Attribute* a : attrs.all({"template", "t"}))
        templates << parse_root(a);
    return true;
}

namespace {
    void    write_onto(Attribute& a, const QuillData::RootData& r)
    {
        a.data = r.path;
        if(!r.key.empty())
            a << Attribute("key", r.key);
        if(!r.name.empty())
            a << Attribute("name", r.name);
        if(!r.color.empty())
            a << Attribute("color", r.color);
        if(r.vcs != Vcs())
            a << Attribute("vcs", r.vcs.key());
        for(DataRole dr : DataRole::all_values()){
            if(r.policy[dr] != Access())
                a << Attribute(dr.key(), r.policy[dr].key());
        }
    }
}

bool    QuillFile::write(Vector<char>&chars) 
{
    AttrTree        attrs;

    if(!name.empty())
        attrs << Attribute("name", name);
    if(!abbr.empty())
        attrs << Attribute("abbr", abbr);
    if(!author.empty())
        attrs << Attribute("author", author);
    if(!aux_ports.empty())
        attrs << Attribute("aux", join_string(aux_ports, ", "));
    if(!cache.empty())
        attrs << Attribute("cache", cache);
    if(!copyright.empty()){
        Attribute  a;
        switch(copyright.stance){
        case AssertDeny::Neither:
            a.key   = "notice";
            break;
        case AssertDeny::Deny:
            a.key   = "disclaim";
            break;
        case AssertDeny::Assert:
            a.key   = "copyright";
            break;
        default:
            break;
        }
        if(!copyright.text.empty())
            a.data = copyright.text;
        if(copyright.from)
            a << Attribute("from", String::number(copyright.from));
        if(copyright.to)
            a << Attribute("to", String::number(copyright.to));
        attrs << a;
    }
    if(!home.empty())
        attrs << Attribute("home", home);
    if(!ini.empty())
        attrs << Attribute("ini", ini);
    if(!local_user.empty())
        attrs << Attribute("local", local_user);
    if(!log_dir.empty())
        attrs << Attribute("logs", log_dir);
    if(port)
        attrs << Attribute("port", String::number(port));
    if(read_timeout)
        attrs << Attribute("timeout", String::number(read_timeout));
    if(!temp_dir.empty())
        attrs << Attribute("temp", temp_dir);
    
    for(const auto & r : roots){
        Attribute   a;
        a.key   = "root";
        write_onto(a, r);
        attrs << a;
    }
    for(const auto& r : templates){
        Attribute   a;
        a.key   = "template";
        write_onto(a, r);
        attrs << a;
    }
    
    attrs.write(chars);
    return true;
}



//String              QuillFile::abbreviation() const
//{
    //return value(zAbbr);
//}

//String              QuillFile::author() const
//{
    //return value(zAuthor);
//}

//Set<uint16_t>       QuillFile::aux_ports() const
//{
    //Set<uint16_t>   ret;
    //for(String & s : values(zAux)){
        //uint16_r    r       = s.to_uint16();
        //if(r.good)
            //ret << r.value;
    //}
    //return ret;
//}

//String              QuillFile::cache() const
//{
    //return value(zCache);
//}

//Copyright           QuillFile::copyright() const
//{
    //Copyright   ret;
    //const Attribute   *a  = nullptr;;
    //if((a = first(zDisclaimer))){
        //ret.stance  = AssertDeny::Deny;
    //} else if((a = first(zNotice))){
        //ret.stance  = AssertDeny::Neither;
    //} else if((a = first(zCopyright))){
        //ret.stance  = AssertDeny::Assert;
    //}
    //if(a){
        //ret.text    = a->data;
        //ret.from    = a->value(zFrom).to_ushort().value;
        //ret.to      = a->value(zTo).to_ushort().value;
    //}
    //return ret;
//}

//String              QuillFile::home() const
//{
    //return value(zHome);
//}

//String              QuillFile::ini() const
//{
    //return value(zIni);
//}

//String              QuillFile::local_user() const
//{
    //return value(zLocalUser);
//}

//String              QuillFile::log_dir() const
//{
    //return value(zLogDir);
//}

//String              QuillFile::name() const
//{
    //return value(zName);
//}

//uint16_r            QuillFile::port() const
//{
    //return value(zPort).to_ushort();
//}

//unsigned_r          QuillFile::read_timeout() const
//{
    //return value(zReadTimeout).to_uinteger();
//}


//Vector<QuillFile::RootInfo>    QuillFile::roots() const
//{
    //Vector<RootInfo> ret;
    //for(const Attribute* a : all(zRoot)){
        //if(a->data.empty())
            //continue;
        //ret << RootInfo(a);
    //}
    //return ret;
//}


//String              QuillFile::temp_dir() const
//{
    //return value(zTempDir);
//}


////! List of decalred templates
//Vector<QuillFile::RootInfo>    QuillFile::templates() const
//{
    //Vector<RootInfo> ret;
    //for(const Attribute* a : all(zTemplate)){
        //if(a->data.empty())
            //continue;
        //ret << RootInfo(a);
    //}
    //return ret;
//}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

//QuillFile::RootInfo::RootInfo() = default;

//QuillFile::RootInfo::RootInfo(const String& p) : path(p)
//{
//}

//QuillFile::RootInfo::~RootInfo() = default;

//QuillFile::RootInfo::RootInfo(const Attribute*a)
//{
    //assert(a);
    //if(!a)
        //return ;

    //path    = a->data;
    //key     = a->value(zKey);
    //color   = a->value(zColor);
    //vcs     = Vcs(a->value(zVcs));
    //name    = a->value(zName);
    //for(DataRole dr : DataRole::all_values())
        //policy[dr]  = Access(a->value(dr.key()));
//}
