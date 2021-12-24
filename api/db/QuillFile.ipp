////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

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
    QuillData::Root    parse_root(const KeyValue* a)
    {
        QuillData::Root ret;
        ret.path    = a->data;
        ret.key     = a->value({ "key", "k" });
        ret.color   = a->value("color");
        ret.vcs     = Vcs(a->value("vcs"));
        ret.name    = a->value("name");
        ret.icon    = a->value("icon");
        for(DataRole dr : DataRole::all_values())
            ret.policy[dr]  = Access(a->value(dr.key()));
        return ret;
    }
}

bool    QuillFile::read(ByteArray&&buffer, const std::string& fname) 
{
    KVTree        attrs;
    if(!attrs.parse(buffer, nullptr, true, fname))
        return false;
    
    abbr            = attrs.value({ "abbr", "abbreviation" });
    author          = attrs.value("author");
    aux_ports       = attrs.values_set_u16({"aux"});
    cache           = attrs.value("cache");
    
    const KeyValue   *a  = nullptr;;
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
    name            = attrs.value({"name", "%"});
    port            = attrs.value("port").to_uint16().value;
    read_timeout    = attrs.value("timeout").to_uinteger().value;
    temp_dir        = attrs.value({"temp", "tmp", "tempdir", "temp_dir"});
    
    for(const KeyValue* a : attrs.all({"root", "r"}))
        roots << parse_root(a);
    for(const KeyValue* a : attrs.all({"template", "t"}))
        templates << parse_root(a);
    return true;
}

namespace {
    void    write_onto(KeyValue& a, const QuillData::Root& r)
    {
        a.data = r.path;
        if(!r.key.empty())
            a << KeyValue("key", r.key);
        if(!r.name.empty())
            a << KeyValue("name", r.name);
        if(!r.color.empty())
            a << KeyValue("color", r.color);
        if(!r.icon.empty())
            a << KeyValue("icon", r.icon);
        if(r.vcs != Vcs())
            a << KeyValue("vcs", r.vcs.key());
        for(DataRole dr : DataRole::all_values()){
            if(r.policy[dr] != Access())
                a << KeyValue(dr.key(), r.policy[dr].key());
        }
    }
}

bool    QuillFile::write(Stream&chars) const
{
    KVTree        attrs;

    if(!name.empty())
        attrs << KeyValue("name", name);
    if(!abbr.empty())
        attrs << KeyValue("abbr", abbr);
    if(!author.empty())
        attrs << KeyValue("author", author);
    if(!aux_ports.empty())
        attrs << KeyValue("aux", join(aux_ports, ", "));
    if(!cache.empty())
        attrs << KeyValue("cache", cache);
    if(!copyright.empty()){
        KeyValue  a;
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
            a << KeyValue("from", String::number(copyright.from));
        if(copyright.to)
            a << KeyValue("to", String::number(copyright.to));
        attrs << a;
    }
    if(!home.empty())
        attrs << KeyValue("home", home);
    if(!ini.empty())
        attrs << KeyValue("ini", ini);
    if(!local_user.empty())
        attrs << KeyValue("local", local_user);
    if(!log_dir.empty())
        attrs << KeyValue("logs", log_dir);
    if(port)
        attrs << KeyValue("port", String::number(port));
    if(read_timeout)
        attrs << KeyValue("timeout", String::number(read_timeout));
    if(!temp_dir.empty())
        attrs << KeyValue("temp", temp_dir);
    
    for(const auto & r : roots){
        KeyValue   a;
        a.key   = "root";
        write_onto(a, r);
        attrs << a;
    }
    for(const auto& r : templates){
        KeyValue   a;
        a.key   = "template";
        write_onto(a, r);
        attrs << a;
    }
    
    attrs.write(chars);
    return true;
}

