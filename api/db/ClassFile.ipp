////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

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
    tags += b.tags;
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
    void        read_trigger(const KeyValue* a, ClassData::Trigger& t)
    {
        t.type      = a->data;
        t.name      = a->value({"name"}, true);
        t.brief     = a->value({"brief", "desc"}, true);
        t.notes     = a->value({"note", "notes"}, true);
        
        for(const KeyValue& b : a->subs){
            if(b.cmd == "?")
                t.args[b.key]  = b.data;
        }
    }
    
    void        read_value(const KeyValue* a, ClassData::ValueInfo& v)
    {
        v.brief     = a->value({"desc", "brief"});
        v.notes     = a->value({"notes", "note"});
    }
    
    void        read_field(const KeyValue* a, ClassData::Field&f)
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
        f.tags          = a->values_set({"tag"});
        f.multiplicity  = Multiplicity(a->value("multiplicity"));
        f.restriction   = Restriction(a->value("restriction"));
        f.max_count     = a->value({"max_count", "count"}).to_uint64().value;
        for(const KeyValue* b : a->all({"value", "val"})){
            if(b->data.empty()){
                yNotice() << "Empty value in field, skipped.";
                continue;
            }
            read_value(b, f.values[a->data]);
        }
        for(const KeyValue* b : a->all({"trigger"})){
            ClassData::Trigger t;
            read_trigger(b, t);
            f.triggers << t;
        }
    }
}

bool    ClassFile::read(ByteArray&&buffer, const std::string& fname) 
{
    KVTree        attrs;
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
    tags            = attrs.values_set({"tag"});
    
    for(const KeyValue* a : attrs.all({"field"})){
        if(a->data.empty()){
            yNotice() << "Empty field key skipped.";
            continue;
        }
        read_field(a, fields[a->data]);
    }

    for(const KeyValue* a : attrs.all({"trigger"})){
        Trigger     t;
        read_trigger(a, t);
        triggers << t;
    }


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
            a << KeyValue("count", String::number(f.max_count));
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

bool    ClassFile::write(Stream& chars)   const
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

