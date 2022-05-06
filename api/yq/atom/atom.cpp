////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Atom.hpp"
#include "ClassData.hpp"
#include "ClassFile.hpp"
#include "FieldData.hpp"
#include "FieldFile.hpp"
#include "ValueData.hpp"
#include "ValueFile.hpp"

#include <yq/bit/KeyValue.hpp>
#include <yq/collection/c_utils.hpp>
#include <yq/io/Strings.hpp>
#include <yq/io/XmlUtils.hpp>
#include <yq/text/text_utils.hpp>

#include <yq/db/IDLock.ipp>

namespace yq {
    template class IDLock<Atom>;
    template class IDLock<Class>;
    template class IDLock<Field>;

////////////////////////////////////////////////////////////////////////////////
//  (ATOM)
////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////
//  (ATOM) CLASS(IFICATIONS)
////////////////////////////////////////////////////////////////////////////////


    Class::Data&      Class::Data::merge(const Class::Data&b, bool fOverride)
    {
        set_if_empty(name, b.name, fOverride);
        set_if_empty(plural, b.plural, fOverride);
        set_if_empty(brief, b.brief, fOverride);
        set_if_empty(notes, b.notes, fOverride);
        set_if_empty(folder, b.folder, fOverride);
        set_if_empty(category, b.category, fOverride);
        use += b.use;
        reverse += b.reverse;
        sources += b.sources;
        targets += b.targets;
        set_if_empty(binding, b.binding, fOverride);
        suffixes += b.suffixes;
        prefixes += b.prefixes;
        aliases += b.aliases;
        tags += b.tags;
        return *this;
    }

////////////////////////////////////////////////////////////////////////////////

    void    Class::File::reset() 
    {
        *(Data*) this   = Data{};
    }

    
    #if CLASS_XML_RESAVE
    bool    Class::File::read(const XmlDocument&doc, std::string_view fname) 
    {
        const XmlNode*  xn  = doc.first_node(szYQClass);
        if(!xn){
            yError() << "No appropriate root element!  In: '" << fname << "'";
            return false;
        }
        
        xn      = xn -> first_node(szClass);
        if(!xn){
            yError() << "No class! In: " << fname << "'";
            return false;
        }
        
        name            = read_child(xn, szName, x_sstring);
        plural          = read_child(xn, szPlural, x_sstring);
        brief           = read_child(xn, szBrief, x_sstring);
        notes           = read_child(xn, szNotes, x_sstring);
        folder          = read_child(xn, szFolder, x_sstring);
        use             = read_child_string_set(xn, szIs);
        reverse         = read_child_string_set(xn, szReverse);
        sources         = read_child_string_set(xn, szSource);
        targets         = read_child_string_set(xn, szTarget);
        aliases         = read_child_string_set(xn, szAlias);
        binding         = read_child(xn, szBinding, x_sstring);
        prefixes        = read_child_string_set(xn, szPrefix);
        suffixes        = read_child_string_set(xn, szSuffix);
        tags            = read_child_string_set(xn, szTag);
        category        = read_child(xn, szCategory, x_sstring);
        return true;
    }
    #endif
    
    bool    Class::File::read(KVTree&&attrs, std::string_view fname) 
    {
        name            = attrs.value(kv::key({"%", "name"}));
        plural          = attrs.value("plural");
        brief           = attrs.value("brief");
        notes           = attrs.value("notes");
        folder          = attrs.value("folder");
        use            += attrs.values_set("is");
        reverse        += attrs.values_set("reverse");
        sources        += attrs.values_set("source");
        targets        += attrs.values_set("target");
        aliases        += attrs.values_set("alias");
        binding         = attrs.value("binding");
        prefixes       += attrs.values_set("prefix");
        suffixes       += attrs.values_set("suffix");
        tags           += attrs.values_set("tag");
        category        = attrs.value("category");
        return true;
    }
    
    bool    Class::File::write(KVTree&attrs) const 
    {
        if(!name.empty())
            attrs.set("%", name);
        if(!plural.empty())
            attrs.set("plural", plural);
        if(!brief.empty())
            attrs.set("brief", brief);
        if(!notes.empty())
            attrs.set("notes", notes);
        if(!use.empty())
            attrs.set("is", join(use, ","));
        if(!reverse.empty())
            attrs.set("reverse", join(reverse, ","));
        if(!targets.empty())
            attrs.set("target", join(targets, ","));
        if(!sources.empty())
            attrs.set("source", join(sources, ","));
        if(!prefixes.empty())
            attrs.set("prefix", join(prefixes, ","));
        if(!suffixes.empty())
            attrs.set("suffix", join(suffixes, ","));
        if(!aliases.empty())
            attrs.set("alias", join(aliases, ",") );
        if(!binding.empty())
            attrs.set("binding", binding);
        if(!tags.empty())
            attrs.set("tag", join(tags, ","));
        if(!category.empty())
            attrs.set("category", category);
        return true;
    }



////////////////////////////////////////////////////////////////////////////////
//  (ATOM) FIELD DEFINITIONS
////////////////////////////////////////////////////////////////////////////////

    Field::Data&  Field::Data::merge(const Data&b, bool fOverride)
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
        //triggers += b.triggers;
        //for(const auto &i : b.values){
            //auto j = values.find(i.first);
            //if(j != values.end())
                //j->second.merge(i.second, fOverride);
            //else
                //values[i.first] = i.second;
        //}
        return *this;
    }

////////////////////////////////////////////////////////////////////////////////

    void Field::File::reset() 
    {
        (*(Data*) this) = Data{};
    }

    #if FIELD_XML_RESAVE
    bool    Field::File::read(const XmlDocument& doc, std::string_view fname) 
    {
        const XmlNode*  xn  = doc.first_node(szYQField);
        if(!xn){
            yError() << "No appropriate root element!  In: '" << fname << "'";
            return false;
        }
        
        xn      = xn -> first_node(szField);
        if(!xn){
            yError() << "No field! In: " << fname << "'";
            return false;
        }

        pkey            = read_child(xn, szPKey, x_sstring);
        name            = read_child(xn, szName, x_sstring);
        plural          = read_child(xn, szPlural, x_sstring);
        brief           = read_child(xn, szBrief, x_sstring);
        notes           = read_child(xn, szNotes, x_sstring);
        category        = read_child(xn, szCategory, x_sstring);
        aliases         = read_child_string_set(xn, szAlias);
        classes         = read_child_string_set(xn, szClass);
        types           = read_child_string_set(xn, szType);
        atoms           = read_child_string_set(xn, szAtom);
        expected        = read_child(xn, szExpected, x_sstring);
        tags            = read_child_string_set(xn, szTag);
        multiplicity    = read_child(xn, szMultiple, x_enum<Multiplicity>);
        restriction     = read_child(xn, szRestrict, x_enum<Restriction>);
        max_count       = read_child(xn, szMaxCount, x_uint64).value;
        return true;
    }
    #endif

    bool    Field::File::read(KVTree&&attrs, std::string_view fname) 
    {
        name            = attrs.value(kv::key({ "%", "name" }));
        pkey            = attrs.value("pkey");
        plural          = attrs.value("plural");
        brief           = attrs.value("brief");
        notes           = attrs.value("notes");
        category        = attrs.value("category");
        aliases        += attrs.values_set("alias");
        tags           += attrs.values_set("tag");
        classes        += attrs.values_set("class");
        types          += attrs.values_set("type");
        atoms          += attrs.values_set("atom");
        expected        = attrs.value("expected");
        multiplicity    = Multiplicity(attrs.value("multiple"));
        restriction     = Restriction(attrs.value("restrict"));
        max_count       = to_uint(attrs.value("max")).value;
        return true;
    }
    
    bool    Field::File::write(KVTree&attrs) const 
    {
        if(!name.empty())
            attrs.set("%", name);
        if(!pkey.empty())
            attrs.set("pkey", pkey);
        if(!plural.empty())
            attrs.set("plural", plural);
        if(!brief.empty())
            attrs.set("brief", brief);
        if(!notes.empty())
            attrs.set("notes", notes);
        if(category.empty())
            attrs.set("category", category);
        if(!classes.empty())
            attrs.set("class", join(classes, ","));
        if(!aliases.empty())
            attrs.set("alias", join(aliases, ","));
        if(!tags.empty())
            attrs.set("tag", join(tags, ","));
        if(!types.empty())
            attrs.set("type", join(types, ","));
        if(!atoms.empty())
            attrs.set("atom", join(atoms, ","));
        
        if(multiplicity != Multiplicity())
            attrs.set("multiple", multiplicity.key());
        if(restriction != Restriction())
            attrs.set("restrict", restriction.key());
        if(max_count)
            attrs.set("max", to_string(max_count));
        return true;
    }

////////////////////////////////////////////////////////////////////////////////
//  (ATOM) PROPERTIES
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  (ATOM FIELD) VALUES
////////////////////////////////////////////////////////////////////////////////


    Value::Data&   Value::Data::merge(const Data&b, bool fOverride)
    {
        set_if_empty(notes, b.notes, fOverride);
        set_if_empty(brief, b.brief, fOverride);
        set_if_empty(data, b.data, fOverride);
        return *this;
    }

    
    Value::Data x_value(const XmlNode*xn)
    {
        Value::Data ret;
        read_xn(ret, xn);
        return ret;
    }
    
    bool    read_xn(Value::Data&ret, const XmlNode*xn)
    {
        ret.notes   = read_child(xn, szNotes, x_sstring);
        ret.brief   = read_child(xn, szBrief, x_sstring);
        ret.data    = read_child(xn, szData, x_sstring);
        return true;
    }
    
    void    write_xn(XmlNode*xn, const Value::Data&v)
    {
        write_child(xn, szData, v.data);
        if(!v.brief.empty())
            write_child(xn, szBrief, v.brief);
        if(!v.notes.empty())
            write_child(xn, szNotes, v.notes);
    }

    Value::ListData&   Value::ListData::merge(const ListData&b, bool fOverride)
    {
        values += b.values;
        return *this;
    }

////////////////////////////////////////////////////////////////////////////////


    bool    Value::ListFile::read(const XmlDocument&doc, std::string_view fname)
    {
        const XmlNode*  xn  = doc.first_node(szYQValues);
        if(!xn){
            yError() << "No appropriate root element!  In: '" << fname << "'";
            return false;
        }
        
        values  = read_children(xn, szValue, x_value);
        return true;
    }
    
    bool    Value::ListFile::write(XmlDocument&doc) const
    {
        XmlNode*  xroot  = doc.allocate_node(rapidxml::node_element, szYQUser);
        doc.append_node(xroot);
        
        write_children(xroot, szValue, values);
        return true;
    }

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
