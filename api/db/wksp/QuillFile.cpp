////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "QuillFile.hpp"
#include <db/bit/KeyValue.hpp>

namespace yq {

    void    QuillData::reset()
    {
        *this = {};
    }



    void    QuillFile::reset() 
    {
        QuillData::reset();
    }

    namespace {
        QuillData::Root    parse_root(const KeyValue& a)
        {
            QuillData::Root ret;
            ret.path    = a.data;
            ret.key     = a.value({ "key", "k" });
            ret.color   = a.value("color");
            ret.vcs     = Vcs(a.value("vcs"));
            ret.name    = a.value("name");
            ret.icon    = a.value("icon");
            for(DataRole dr : DataRole::all_values())
                ret.policy[dr]  = Access(a.value(dr.key()));
            return ret;
        }
    }

    bool    QuillFile::read(ByteArray&&buffer, const std::string_view& fname) 
    {
        KVTree        attrs;
        if(!attrs.parse(buffer, nullptr, true, fname))
            return false;
        
        abbr            = attrs.value({ "abbr", "abbreviation" });
        author          = attrs.value("author");
        aux_ports       = attrs.values_set_u16("aux");
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
            copyright.from    = to_ushort(a->value("from")).value;
            copyright.to      = to_ushort(a->value("to")).value;
        }
        
        home            = attrs.value("home");
        ini             = attrs.value("ini");
        local_user      = attrs.value({"local", "user", "local_user"});
        log_dir         = attrs.value("logs");
        name            = attrs.value({"name", "%"});
        port            = to_uint16(attrs.value("port")).value;
        read_timeout    = to_uinteger(attrs.value("timeout")).value;
        temp_dir        = attrs.value({"temp", "tmp", "tempdir", "temp_dir"});
        
        attrs.all(kv::key({"root", "r"}), [&](const KeyValue& a){
            roots << parse_root(a);
        });
        attrs.all(kv::key({"template", "t"}), [&](const KeyValue& a){
            templates << parse_root(a);
        });
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

    bool    QuillFile::write(yq::Stream&chars) const
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

}
