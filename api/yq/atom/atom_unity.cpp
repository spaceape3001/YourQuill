////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "ClassFile.hpp"

#include <yq/bit/KeyValue.hpp>
#include <yq/log/Logging.hpp>
#include <yq/text/Utils.hpp>

namespace yq {

    ClassData&      ClassData::merge(const ClassData&b, bool fOverride)
    {
        set_if_empty(name, b.name, fOverride);
        set_if_empty(plural, b.plural, fOverride);
        set_if_empty(brief, b.brief, fOverride);
        set_if_empty(notes, b.notes, fOverride);
        set_if_empty(folder, b.folder, fOverride);
        use += b.use;
        reverse += b.reverse;
        sources += b.sources;
        targets += b.targets;
        set_if_empty(binding, b.binding, fOverride);
        triggers += b.triggers;
        suffixes += b.suffixes;
        prefixes += b.prefixes;
        aliases += b.aliases;
        tags += b.tags;

        for(const auto& i : b.fields){
            auto j = fields.find(i.first);
            if(j != fields.end()){
                j->second.merge(i.second, fOverride);
            } else {
                fields[i.first] = i.second;
            }
        }
        return *this;
    }

    void            ClassData::reset()
    {
        name.clear();
        plural.clear();
        brief.clear();
        notes.clear();
        folder.clear();
        use.clear();
        reverse.clear();
        sources.clear();
        targets.clear();
        prefixes.clear();
        suffixes.clear();
        binding.clear();
        fields.clear();
        triggers.clear();
        aliases.clear();
        tags.clear();
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ClassData::Field& ClassData::Field::merge(const Field& b, bool fOverride)
    {
        set_if_empty(pkey, b.pkey, fOverride);
        set_if_empty(name, b.name, fOverride);
        set_if_empty(plural, b.plural, fOverride);
        set_if_empty(brief, b.brief, fOverride);
        set_if_empty(notes, b.notes, fOverride);
        set_if_empty(category, b.category, fOverride);
        aliases += b.aliases;
        //type.set_if(b.type, fOverride);
        types += b.types;
        //atom.set_if(b.atom, fOverride);
        atoms += b.atoms;
        tags += b.tags;
        set_if_empty(expected, b.expected, fOverride);
        if((fOverride?b.multiplicity:multiplicity) != Multiplicity())
            multiplicity    = b.multiplicity;
        if((fOverride?b.restriction:restriction) != Restriction())
            restriction     = b.restriction;
        if(fOverride?b.max_count:!max_count)
            max_count       = b.max_count;
        triggers += b.triggers;
        for(const auto &i : b.values){
            auto j = values.find(i.first);
            if(j != values.end())
                j->second.merge(i.second, fOverride);
            else
                values[i.first] = i.second;
        }
        return *this;
    }



    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ClassData::ValueInfo&   ClassData::ValueInfo::merge(const ValueInfo&b, bool fOverride)
    {
        set_if_empty(brief, b.brief, fOverride);
        set_if_empty(notes, b.notes, fOverride);
        return *this;
    }




    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    void    ClassFile::reset() 
    {
        ClassData::reset();
    }


    namespace {
        void        read_trigger(const KeyValue& a, ClassData::Trigger& t)
        {
            t.type      = a.data;
            t.name      = a.value(kv::noncmd_key("name"));
            t.brief     = a.value(kv::noncmd_key("brief", "desc"));
            t.notes     = a.value(kv::noncmd_key("note", "notes"));
            
            for(const KeyValue& b : a.subs){
                if(b.cmd == "?")
                    t.args[b.key]  = b.data;
            }
        }
        
        void        read_value(const KeyValue& a, ClassData::ValueInfo& v)
        {
            v.brief     = a.value(kv::key("desc", "brief"));
            v.notes     = a.value(kv::key("notes", "note"));
        }
        
        void        read_field(const KeyValue& a, ClassData::Field&f)
        {
            f.pkey          = a.value(kv::key("pkey"));
            f.name          = a.value(kv::key("%", "name"));
            f.plural        = a.value(kv::key("plural"));
            f.brief         = a.value(kv::key("desc", "brief"));
            f.notes         = a.value(kv::key("notes", "note"));
            f.category      = a.value(kv::key("category", "cat"));
            f.aliases       = copy(a.values_set(kv::key("alias", "aka")));
            f.types         = copy(a.values_set(kv::key("type")));
            f.expected      = a.value(kv::key("expected", "expect"));
            f.atoms         = copy(a.values_set(kv::key("atom")));
            f.aliases       = copy(a.values_set(kv::key("alias", "aka")));
            f.tags          = copy(a.values_set(kv::key("tag")));
            f.multiplicity  = Multiplicity(a.value(kv::key("multiplicity")));
            f.restriction   = Restriction(a.value(kv::key("restriction")));
            f.max_count     = to_uint64(a.value(kv::key("max_count", "count"))).value;
            
            a.all(kv::key({"value", "val"}), [&](const KeyValue& b){
                if(b.data.empty()){
                    yNotice() << "Empty value in field, skipped.";
                    return;
                }
                read_value(b, f.values[a.data]);
            });
            
            a.all(kv::key("trigger"), [&](const KeyValue& b){
                ClassData::Trigger t;
                read_trigger(b, t);
                f.triggers << t;
            });
        }
    }

    bool    ClassFile::read(ByteArray&&buffer, const std::string_view& fname) 
    {
        KVTree        attrs;
        if(!attrs.parse(buffer, nullptr, true, fname))
            return false;

        name            = attrs.value(kv::key("%", "name"));
        plural          = attrs.value(kv::key("plural"));
        brief           = attrs.value(kv::key("desc", "brief"));
        notes           = attrs.value(kv::key("notes", "note"));
        folder          = attrs.value(kv::key("folder"));
        use             = copy(attrs.values_set(kv::key("use", "is")));
        reverse         = copy(attrs.values_set(kv::key("reverse", "rev")));
        sources         = copy(attrs.values_set(kv::key("sources", "source", "src")));
        targets         = copy(attrs.values_set(kv::key("targets", "target", "tgt")));
        aliases         = copy(attrs.values_set(kv::key("alias", "aliases", "aka")));
        binding         = attrs.value(kv::key("binding"));
        prefixes        = copy(attrs.values_set(kv::key("prefix")));
        suffixes        = copy(attrs.values_set(kv::key("suffix")));
        tags            = copy(attrs.values_set(kv::key("tag")));
        
        attrs.all(kv::key("field"), [&](const KeyValue& a){
            if(a.data.empty()){
                yNotice() << "Empty field key skipped.";
                return;
            }
            read_field(a, fields[a.data]);
        });
        attrs.all(kv::key("trigger"), [&](const KeyValue& a){
            Trigger     t;
            read_trigger(a, t);
            triggers << t;
        });

        return true;
    }

    namespace {
        void    write_trigger(KeyValue& a, const ClassData::Trigger& t)
        {
            if(!t.name.empty())
                a << KeyValue("name", t.name);
            if(!t.brief.empty())
                a << KeyValue("desc", t.brief);
            if(!t.notes.empty())
                a << KeyValue("notes", t.notes);
            for(auto& i : t.args){
                KeyValue b(i.first, i.second);
                b.cmd = "?";
                a << b;
            }
        }
        
        void    write_value(KeyValue& a, const ClassData::ValueInfo& v)
        {
            if(!v.brief.empty())
                a << KeyValue("desc", v.brief);
            if(!v.notes.empty())
                a << KeyValue("notes", v.notes);
        }
        
        void    write_field(KeyValue& a, const ClassData::Field& f)
        {
            if(!f.pkey.empty())
                a << KeyValue("pkey", f.pkey);
            if(!f.name.empty())
                a << KeyValue("name", f.name);
            if(!f.plural.empty())   
                a << KeyValue("plural", f.plural);
            if(!f.brief.empty())
                a << KeyValue("desc", f.brief);
            if(!f.notes.empty())
                a << KeyValue("notes", f.notes);
            if(!f.category.empty())
                a << KeyValue("category", f.category);
            if(!f.aliases.empty())
                a << KeyValue("alias", join(f.aliases, ", "));
            if(!f.types.empty())
                a << KeyValue("type", join(f.types, ", "));
            if(!f.atoms.empty())
                a << KeyValue("atom", join(f.atoms, ", "));
            if(!f.tags.empty())
                a << KeyValue("tag", join(f.tags, ", "));
            if(!f.expected.empty())
                a << KeyValue("expected", f.expected);
            if(f.multiplicity != Multiplicity())
                a << KeyValue("multiplicity", f.multiplicity.key());
            if(f.restriction != Restriction())
                a << KeyValue("restriction", f.restriction.key());
            if(f.max_count)
                a << KeyValue("count", to_string(f.max_count));
            for(auto& i : f.values){
                KeyValue   b("value", i.first);
                write_value(b, i.second);
                a << b;
            }
            for(auto& j : f.triggers){
                KeyValue b("trigger", j.type);
                write_trigger(b, j);
                a << b;
            }
        }
    }

    bool    ClassFile::write(yq::Stream& chars)   const
    {
        KVTree        attrs;
        
        if(!name.empty())
            attrs << KeyValue("name", name);
        if(!plural.empty())
            attrs << KeyValue("plural", plural);
        if(!brief.empty())
            attrs << KeyValue("desc", brief);
        if(!notes.empty())
            attrs << KeyValue("notes", notes);
        if(!use.empty())
            attrs << KeyValue("use", join(use, ", "));
        if(!reverse.empty())
            attrs << KeyValue("reverse", join(reverse, ", "));
        if(!targets.empty())
            attrs << KeyValue("target", join(targets, ", "));
        if(!sources.empty())
            attrs << KeyValue("source", join(sources, ", "));
        if(!prefixes.empty())
            attrs << KeyValue("prefix", join(prefixes, ", "));
        if(!suffixes.empty())
            attrs << KeyValue("suffix", join(suffixes, ", "));
        if(!aliases.empty())
            attrs << KeyValue("alias", join(aliases, ", "));
        if(!binding.empty())
            attrs << KeyValue("binding", binding);
        if(!tags.empty())
            attrs << KeyValue("tag", join(tags, ", "));
        for(auto& t : triggers){
            KeyValue   a("trigger", t.type);
            write_trigger(a, t);
            attrs << a;
        }
        for(auto& i : fields){
            KeyValue   a("field", i.first);
            write_field(a, i.second);
            attrs << a;
        }
            
        attrs.write(chars);
        return true;
    }

}
