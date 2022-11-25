////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FieldFile.hpp"
#include <basic/XmlUtils.hpp>
#include <kernel/bit/KeyValue.hpp>
#include <kernel/io/Strings.hpp>

namespace yq {
    namespace agw {

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

            pkey            = read_child(xn, szPKey, x_string);
            name            = read_child(xn, szName, x_string);
            plural          = read_child(xn, szPlural, x_string);
            brief           = read_child(xn, szBrief, x_string);
            notes           = read_child(xn, szNotes, x_string);
            category        = read_child(xn, szCategory, x_string);
            aliases         = read_child_string_set(xn, szAlias);
            classes         = read_child_string_set(xn, szClass);
            types           = read_child_string_set(xn, szType);
            atoms           = read_child_string_set(xn, szAtom);
            expected        = read_child(xn, szExpected, x_string);
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
    }
}
