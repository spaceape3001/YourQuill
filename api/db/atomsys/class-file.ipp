////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace yq {
    void    Class::File::reset() 
    {
        *(Data*) this   = Data{};
    }

    
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
    

    bool    Class::File::write(XmlDocument&doc) const
    {
        XmlNode*  xroot  = doc.allocate_node(rapidxml::node_element, szYQClass);
        doc.append_node(xroot);
        XmlNode*    xn   = doc.allocate_node(rapidxml::node_element, szClass);
        xroot -> append_node(xn);

        if(!name.empty())
            write_child(xn, szName, name);
        if(!plural.empty())
            write_child(xn, szPlural, name);
        if(!brief.empty())
            write_child(xn, szBrief, brief);
        if(!notes.empty())
            write_child(xn, szNotes, notes);
        if(!use.empty())
            write_children(xn, szIs, use);
        if(!reverse.empty())
            write_children(xn, szReverse, reverse);
        if(!targets.empty())
            write_children(xn, szTarget, targets);
        if(!sources.empty())
            write_children(xn, szSource, sources);
        if(!prefixes.empty())
            write_children(xn, szPrefix, prefixes);
        if(!suffixes.empty())
            write_children(xn, szSuffix, suffixes);
        if(!aliases.empty())
            write_children(xn, szAlias, aliases);
        if(!binding.empty())
            write_child(xn, szBinding, binding);
        if(!tags.empty())
            write_children(xn, szTag, tags);
        if(!category.empty())
            write_child(xn, szCategory, category);
        return true;
    }

}
