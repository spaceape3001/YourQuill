////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "QuillFile.hpp"

#include <yq/keyv/KeyValue.hpp>
#include <mithril/io/Strings.hpp>
#include <yq/text/join.hpp>
#include <yq/text/parse.hpp>
#include <yq/text/transform.hpp>

namespace yq::mithril {
    void    QuillFile::reset() 
    {
        QuillData::reset();
    }

    namespace {
        QuillData::RootDir    parse_root(const KeyValue& a)
        {
            QuillData::RootDir ret;
            ret.path    = trimmed(a.data);
            ret.key     = trimmed(a.value(kv::key( "key", "k" )));
            ret.color   = a.value(kv::key("color"));
            ret.vcs     = Vcs(a.value(kv::key("vcs")));
            ret.name    = a.value(kv::key("name"));
            ret.icon    = a.value(kv::key("icon"));
            for(DataRole dr : DataRole::all_values())
                ret.policy[dr]  = Access(a.value(dr.key()));
            return ret;
        }
    }

    std::error_code     QuillFile::read(KVTree&&attrs, std::string_view) 
    {
        abbr            = attrs.value(kv::key( "abbr", "abbreviation" ));
        author          = attrs.value(kv::key("author"));
        aux_ports       = attrs.values_set_u16("aux");
        cache           = attrs.value(kv::key("cache"));
        
        const KeyValue   *a  = nullptr;;
        if((a = attrs.first(kv::key("disclaimer", "disclaim", "deny")))){
            copyright.stance  = AssertDeny::Deny;
        } else if((a = attrs.first(kv::key("notice")))){
            copyright.stance  = AssertDeny::Neither;
        } else if((a = attrs.first(kv::key("copyright")))){
            copyright.stance  = AssertDeny::Assert;
        }
        if(a){
            copyright.text    = a->data;
            copyright.from    = to_ushort(a->value(kv::key("from"))).value_or(0);
            copyright.to      = to_ushort(a->value(kv::key("to"))).value_or(0);
        }
        
        home            = attrs.value(kv::key("home"));
        ini             = attrs.value(kv::key("ini"));
        local_user      = attrs.value(kv::key("local", "user", "local_user"));
        log_dir         = attrs.value(kv::key("logs"));
        name            = attrs.value(kv::key("name", "%"));
        port            = to_uint16(attrs.value(kv::key("port"))).value_or(0);
        read_timeout    = to_uinteger(attrs.value(kv::key("timeout"))).value_or(0);
        threads         = to_uinteger(attrs.value(kv::key("threads"))).value_or(0);
        temp_dir        = attrs.value(kv::key("temp", "tmp", "tempdir", "temp_dir"));
        
        attrs.all(kv::key("root", "r"), [&](const KeyValue& a){
            root_dirs << parse_root(a);
        });
        attrs.all(kv::key("template", "t"), [&](const KeyValue& a){
            templates << parse_root(a);
        });
        return std::error_code(); 
    }

    namespace {
        void    write_onto(KeyValue& a, const QuillData::RootDir& r)
        {
            a.data = r.path;
            if(!r.key.empty())
                a << KeyValue(szKey, r.key);
            if(!r.name.empty())
                a << KeyValue(szName, r.name);
            if(!r.color.empty())
                a << KeyValue(szColor, r.color);
            if(!r.icon.empty())
                a << KeyValue(szIcon, r.icon);
            if(r.vcs != Vcs())
                a << KeyValue(szVcs, r.vcs.key());
            for(DataRole dr : DataRole::all_values()){
                if(r.policy[dr] != Access())
                    a << KeyValue(dr.key(), r.policy[dr].key());
            }
        }
    }

    std::error_code     QuillFile::write(KVTree& attrs) const
    {
        if(!name.empty())
            attrs << KeyValue(szName, name);
        if(!abbr.empty())
            attrs << KeyValue("abbr", abbr);
        if(!author.empty())
            attrs << KeyValue(szAuthor, author);
        if(!aux_ports.empty())
            attrs << KeyValue("aux", join(aux_ports, ", "));
        if(!cache.empty())
            attrs << KeyValue(szCache, cache);
        if(!copyright.empty()){
            KeyValue  a;
            switch(copyright.stance){
            case AssertDeny::Neither:
                a.key   = szNotice;
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
                a << KeyValue("from", to_string(copyright.from));
            if(copyright.to)
                a << KeyValue("to", to_string(copyright.to));
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
            attrs << KeyValue("port", to_string(port));
        if(read_timeout)
            attrs << KeyValue("timeout", to_string(read_timeout));
        if(!temp_dir.empty())
            attrs << KeyValue("temp", temp_dir);
        if(threads)
            attrs << KeyValue("threads", to_string(threads));
        
        for(const auto & r : root_dirs){
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
        return std::error_code(); 
    }
}
