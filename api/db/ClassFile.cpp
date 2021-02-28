////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "ClassFile.hpp"
#include <util/Logging.hpp>
#include <util/Utilities.hpp>

ClassData&      ClassData::merge(const ClassData&b, bool fOverride)
{
    name.set_if(b.name, fOverride);
    plural.set_if(b.plural, fOverride);
    brief.set_if(b.brief, fOverride);
    notes.set_if(b.notes, fOverride);
    folder.set_if(b.folder, fOverride);
    use += b.use;
    reverse += b.reverse;
    sources += b.sources;
    targets += b.targets;
    binding.set_if(b.binding, fOverride);
    triggers += b.triggers;
    suffixes += b.suffixes;
    prefixes += b.prefixes;
    aliases += b.aliases;

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
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ClassData::Field& ClassData::Field::merge(const Field& b, bool fOverride)
{
    pkey.set_if(b.pkey, fOverride);
    name.set_if(b.name, fOverride);
    plural.set_if(b.plural, fOverride);
    brief.set_if(b.brief, fOverride);
    notes.set_if(b.notes, fOverride);
    category.set_if(b.category, fOverride);
    aliases += b.aliases;
    //type.set_if(b.type, fOverride);
    types += b.types;
    //atom.set_if(b.atom, fOverride);
    atoms += b.atoms;
    expected.set_if(b.expected, fOverride);
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
    brief.set_if(b.brief, fOverride);
    notes.set_if(b.notes, fOverride);
    return *this;
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void    ClassFile::reset() 
{
    ClassData::reset();
}


namespace {
    void        read_trigger(const Attribute* a, ClassData::Trigger& t)
    {
        t.type      = a->data;
        t.name      = a->value({"name"}, true);
        t.brief     = a->value({"brief", "desc"}, true);
        t.notes     = a->value({"note", "notes"}, true);
        
        for(const Attribute& b : a->attrs){
            if(b.cmd == "?")
                t.args[b.key]  = b.data;
        }
    }
    
    void        read_value(const Attribute* a, ClassData::ValueInfo& v)
    {
        v.brief     = a->value({"desc", "brief"});
        v.notes     = a->value({"notes", "note"});
    }
    
    void        read_field(const Attribute* a, ClassData::Field&f)
    {
        f.pkey          = a->value("pkey");
        f.name          = a->value({"%", "name"});
        f.plural        = a->value("plural");
        f.brief         = a->value({"desc", "brief"});
        f.notes         = a->value({"notes", "note"});
        f.category      = a->value({"category", "cat"});
        f.aliases       = a->values_set({"alias", "aka"});
        f.types         = a->values_set({"type"});
        f.expected      = a->value({"expected", "expect"});
        f.atoms         = a->values_set({"atom"});
        f.aliases       = a->values_set({"alias", "aka"});
        f.multiplicity  = Multiplicity(a->value("multiplicity"));
        f.restriction   = Restriction(a->value("restriction"));
        f.max_count     = a->value({"max_count", "count"}).to_uint64().value;
        for(const Attribute* b : a->all({"value", "val"})){
            if(b->data.empty()){
                yNotice() << "Empty value in field, skipped.";
                continue;
            }
            read_value(b, f.values[a->data]);
        }
        for(const Attribute* b : a->all({"trigger"})){
            ClassData::Trigger t;
            read_trigger(b, t);
            f.triggers << t;
        }
    }
}

bool    ClassFile::read(Vector<char>&buffer, const std::string& fname) 
{
    AttrTree        attrs;
    if(!attrs.parse(buffer, nullptr, true, fname))
        return false;

    name            = attrs.value({"%", "name"});
    plural          = attrs.value({"plural"});
    brief           = attrs.value({"desc", "brief"});
    notes           = attrs.value({"notes", "note"});
    folder          = attrs.value({"folder"});
    use             = attrs.values_set({"use", "is"});
    reverse         = attrs.values_set({"reverse", "rev"});
    sources         = attrs.values_set({"sources", "source", "src"});
    targets         = attrs.values_set({"targets", "target", "tgt"});
    aliases         = attrs.values_set({"alias", "aliases", "aka"});
    binding         = attrs.value({"binding"});
    prefixes        = attrs.values_set({"prefix"});
    suffixes        = attrs.values_set({"suffix"});
    
    for(const Attribute* a : attrs.all({"field"})){
        if(a->data.empty()){
            yNotice() << "Empty field key skipped.";
            continue;
        }
        read_field(a, fields[a->data]);
    }

    for(const Attribute* a : attrs.all({"trigger"})){
        Trigger     t;
        read_trigger(a, t);
        triggers << t;
    }


    return true;
}

namespace {
    void    write_trigger(Attribute& a, const ClassData::Trigger& t)
    {
        if(!t.name.empty())
            a << Attribute("name", t.name);
        if(!t.brief.empty())
            a << Attribute("desc", t.brief);
        if(!t.notes.empty())
            a << Attribute("notes", t.notes);
        for(auto& i : t.args){
            Attribute b(i.first, i.second);
            b.cmd = "?";
            a << b;
        }
    }
    
    void    write_value(Attribute& a, const ClassData::ValueInfo& v)
    {
        if(!v.brief.empty())
            a << Attribute("desc", v.brief);
        if(!v.notes.empty())
            a << Attribute("notes", v.notes);
    }
    
    void    write_field(Attribute& a, const ClassData::Field& f)
    {
        if(!f.pkey.empty())
            a << Attribute("pkey", f.pkey);
        if(!f.name.empty())
            a << Attribute("name", f.name);
        if(!f.plural.empty())   
            a << Attribute("plural", f.plural);
        if(!f.brief.empty())
            a << Attribute("desc", f.brief);
        if(!f.notes.empty())
            a << Attribute("notes", f.notes);
        if(!f.category.empty())
            a << Attribute("category", f.category);
        if(!f.aliases.empty())
            a << Attribute("alias", join(f.aliases, ", "));
        if(!f.types.empty())
            a << Attribute("type", join(f.types, ", "));
        if(!f.atoms.empty())
            a << Attribute("atom", join(f.atoms, ", "));
        if(!f.expected.empty())
            a << Attribute("expected", f.expected);
        if(f.multiplicity != Multiplicity())
            a << Attribute("multiplicity", f.multiplicity.key());
        if(f.restriction != Restriction())
            a << Attribute("restriction", f.restriction.key());
        if(f.max_count)
            a << Attribute("count", String::number(f.max_count));
        for(auto& i : f.values){
            Attribute   b("value", i.first);
            write_value(b, i.second);
            a << b;
        }
        for(auto& j : f.triggers){
            Attribute b("trigger", j.type);
            write_trigger(b, j);
            a << b;
        }
    }
}

bool    ClassFile::write(Vector<char>& chars)  
{
    AttrTree        attrs;
    
    if(!name.empty())
        attrs << Attribute("name", name);
    if(!plural.empty())
        attrs << Attribute("plural", plural);
    if(!brief.empty())
        attrs << Attribute("desc", brief);
    if(!notes.empty())
        attrs << Attribute("notes", notes);
    if(!use.empty())
        attrs << Attribute("use", join(use, ", "));
    if(!reverse.empty())
        attrs << Attribute("reverse", join(reverse, ", "));
    if(!targets.empty())
        attrs << Attribute("target", join(targets, ", "));
    if(!sources.empty())
        attrs << Attribute("source", join(sources, ", "));
    if(!prefixes.empty())
        attrs << Attribute("prefix", join(prefixes, ", "));
    if(!suffixes.empty())
        attrs << Attribute("suffix", join(suffixes, ", "));
    if(!aliases.empty())
        attrs << Attribute("alias", join(aliases, ", "));
    if(!binding.empty())
        attrs << Attribute("binding", binding);
    for(auto& t : triggers){
        Attribute   a("trigger", t.type);
        write_trigger(a, t);
        attrs << a;
    }
    for(auto& i : fields){
        Attribute   a("field", i.first);
        write_field(a, i.second);
        attrs << a;
    }
        
    attrs.write(chars);
    return true;
}

