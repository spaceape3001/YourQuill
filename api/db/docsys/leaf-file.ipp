////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace yq {

    namespace {
        
        using Seq = std::initializer_list<std::string_view>;
        static constexpr Seq zAbbr      = { "%abbr"sv };
        static constexpr Seq zBrief     = { "%desc"sv, "%brief"sv };
        static constexpr Seq zClass     = { "%is"sv, "%cls"sv, "%class"sv, "%use"sv, "%as"sv, "%atom"sv };
        static constexpr Seq zFormat    = { "%fmt"sv, "%format"sv };
        static constexpr Seq zNote      = { "%note"sv, "%notes"sv };
        static constexpr Seq zTag       = { "%tag"sv, "%tags"sv };
        static constexpr Seq zTitle     = { "%"sv, "%leaf"sv, "%title"sv };
        static constexpr Format defFormat   = Format::Markdown;
    }
    
    
    Leaf::KV    x_leaf_kv(const XmlNode* node)
    {
        Leaf::KV          ret;
        read_xn(ret, node);
        return ret;
    }
    
    void        read_xn(Leaf::KV& ret, const XmlNode* node)
    {
        ret.key     = read_attribute(node, szKey, x_sstring);
        ret.cmd     = read_attribute(node, szCmd, x_sstring);
        ret.id      = read_attribute(node, szId, x_sstring);
        ret.type    = read_attribute(node, szType, x_sstring);
        ret.at      = read_attribute(node, szAt, x_sstring);
        ret.until   = read_attribute(node, szUntil, x_sstring);
        ret.data    = x_sstring(node);
        ret.subs    = read_children(node, szAttribute, x_leaf_kv);
    }
    
    void        write_xn(XmlNode* xn, const Leaf::KV& kv)
    {
        write_attribute(xn, szKey, kv.key);
        if(!kv.cmd.empty())
            write_attribute(xn, szCmd, kv.cmd);
        if(!kv.id.empty())
            write_attribute(xn, szId, kv.id);
        if(!kv.at.empty())
            write_attribute(xn, szAt, kv.at);
        if(!kv.until.empty())
            write_attribute(xn, szUntil, kv.until);
        if(!kv.type.empty())
            write_attribute(xn, szType, kv.type);
        if(!kv.data.empty())
            write_x(xn, kv.data);
        write_children(xn, szAttribute, kv.subs);
    }

    void    Leaf::File::reset() 
    {
        *static_cast<Data*>(this) = Data();
    }
    
    bool    Leaf::File::read(ByteArray&&buffer, std::string_view fname) 
    {
        if(starts(buffer.as_view(), "<?xml")){
            XmlFile::read(std::move(buffer), fname);
            return true;
        }
        
        std::string     body;
        KVTree          tree;
        if(!tree.parse(buffer, &body, true, fname))
            return false;

        Format      fmt;
        bool        has_name    = false;
        bool        is_nameable = true;

        for(const KeyValue& kv : tree.subs){
        
            if(kv.key.empty())
                continue;
            if(kv.key[0] == '%'){
                if(is_in(kv.key, zAbbr)){
                    abbr        = kv.data;
                } else if(is_in(kv.key, zBrief)){
                    brief       = kv.data;
                } else if(is_in(kv.key, zClass)){
                    vsplit(kv.data, ',', [&](std::string_view v){
                        v = trimmed(v);
                        if(is_similar(v, "event"))
                            is_nameable = false;
                        if(is_similar(v, "org"))
                            classes.insert("Group");
                        else
                            classes.insert(std::string(v));
                    });
                } else if(is_in(kv.key, zFormat)){
                    fmt         = Format(trimmed(kv.data));
                } else if(is_in(kv.key, zNote)){
                    notes       = trimmed(kv.data);
                } else if(is_in(kv.key, zTag)){
                    vsplit(kv.data, ',', [&](std::string_view v){
                        tags.insert(std::string(trimmed(v)));
                    });
                } else if(is_in(kv.key, zTitle)){
                    title       = kv.data;
                } else if(is_in(kv.key, { "%key" })){
                    //  don't care....
                } else {
                    yWarning() << "Unknown attribute '" << kv.key << "' in: " << fname;
                }
            } else {
                KV      ki;
                ki  = kv;
                attrs.push_back(ki);
                if(is_similar(kv.key, "name"))
                    has_name = true;
            }
        }
        
        if(is_nameable && !has_name){
            KV ki;
            ki.key  = "Name";
            ki.data = title;
            title   = std::string();
            attrs.push_back(ki);
        }
        
        
        std::string_view    btrim = trimmed(body);
        if(!btrim.empty()){
            Context ctx;
            ctx.format  = fmt;
            ctx.data    = body;
            context.push_back(ctx);
        }
        
        save_to(fname);
        return true;
    }
    
    bool    Leaf::File::read(const XmlDocument& doc, std::string_view fname) 
    {
        const XmlNode*  xn  = doc.first_node(szYQLeaf);
        if(!xn){
            yError() << "No appropriate root element!  In: '" << fname << "'";
            return false;
        }
        
        xn      = xn -> first_node(szLeaf);
        if(!xn){
            yError() << "No tag! In: " << fname << "'";
            return false;
        }
        
        title    = read_attribute(xn, szTitle, x_sstring);
        notes    = read_child(xn, szNotes, x_sstring);
        brief    = read_child(xn, szBrief, x_sstring);
        abbr     = read_child(xn, szAbbreviation, x_sstring);
        for(std::string& s : read_children(xn, szClass, x_sstring))
            classes.insert(std::move(s));
        for(std::string& s : read_children(xn, szTag, x_sstring))
            tags.insert(std::move(s));
        attrs    = read_children(xn, szAttribute, x_leaf_kv);
        context  = read_children(xn, szContext, x_context);
        
        return true;
    }
    
    bool    Leaf::File::write(XmlDocument& doc) const 
    {
        XmlNode*  xroot  = doc.allocate_node(rapidxml::node_element, szYQLeaf);
        doc.append_node(xroot);
        XmlNode*    xn   = doc.allocate_node(rapidxml::node_element, szLeaf);
        xroot->append_node(xn);
        if(!title.empty())
            write_attribute(xn, szTitle, title);
        if(!abbr.empty())
            write_child(xn, szAbbr, abbr);
        if(!brief.empty())
            write_child(xn, szBrief, brief);
        if(!notes.empty())
            write_child(xn, szNotes, notes);
        write_children(xn, szClass, classes);
        write_children(xn, szTag, tags);
        write_children(xn, szAttribute, attrs);
        write_children(xn, szContext, context);
        return true;
    }
    
}
