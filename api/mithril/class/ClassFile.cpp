////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "ClassFile.hpp"
#include <yq/xml/XmlUtils.hpp>
#include <yq/keyv/KeyValue.hpp>
#include <yq/text/join.hpp>
#include <mithril/io/Strings.hpp>

namespace yq::errors {
    using no_class_in_file  = error_db::entry<"No class found in the file.">;
}

namespace yq::mithril {

    void    Class::File::reset() 
    {
        *(Data*) this   = Data{};
    }

    
    
    #if CLASS_XML_RESAVE
    std::error_code    Class::File::read(const XmlDocument&doc, std::string_view fname) 
    {
        const XmlNode*  xn  = doc.first_node(szYQClass);
        if(!xn){
            yError() << "No appropriate root element!  In: '" << fname << "'";
            return errors::xml_no_root_element();
        }
        
        xn      = xn -> first_node(szClass);
        if(!xn){
            yError() << "No class! In: " << fname << "'";
            return errors::no_class_in_file();
        }
        
        name            = read_child(xn, szName, x_string);
        plural          = read_child(xn, szPlural, x_string);
        brief           = read_child(xn, szBrief, x_string);
        notes           = read_child(xn, szNotes, x_string);
        folder          = read_child(xn, szFolder, x_string);
        use             = read_child_string_set(xn, szIs);
        reverse         = read_child_string_set(xn, szReverse);
        sources         = read_child_string_set(xn, szSource);
        targets         = read_child_string_set(xn, szTarget);
        aliases         = read_child_string_set(xn, szAlias);
        binding         = read_child(xn, szBinding, x_string);
        prefixes        = read_child_string_set(xn, szPrefix);
        suffixes        = read_child_string_set(xn, szSuffix);
        tags            = read_child_string_set(xn, szTag);
        category        = read_child(xn, szCategory, x_string);
        url             = read_child(xn, szUrl, x_string);
        return std::error_code();
    }
    #endif
    
    std::error_code    Class::File::read(KVTree&&attrs, std::string_view) 
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
        url             = attrs.value("url");
        dev_url         = attrs.value("devurl");
        return std::error_code();
    }
    
    std::error_code    Class::File::write(KVTree&attrs) const 
    {
        if(!name.empty())
            attrs.set("name", name);
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
        if(!url.empty())
            attrs.set("url", url);
        if(!dev_url.empty())
            attrs.set("devurl", dev_url);
        return std::error_code();
    }

}
