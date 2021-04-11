////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    R       determine_leaf(const String&s)
    {
        if(s.empty())
            return R::EMPTY;
            
        x_leaf  = cdb::leaf(s.qString());
        if(x_leaf)
            return R::OKAY;
        auto u  = s.to_uint64();
        if(u.good){
            x_leaf  = cdb::leaf(u.value);
            if(x_leaf)
                return R::OKAY;
        }
        
        x_leaf  = cdb::leaf_by_title(s.qString());
        if(x_leaf)
            return R::OKAY;
            
        return R::NOT_FOUND;
    }

    void    _attributes(HtmlWriter& h, Vector<Attribute>& attrs, size_t d=0)
    {
        attrs.stable_sort([](const Attribute&a, const Attribute& b) -> bool{
            return is_less(compare_igCase(a.key, b.key));
        });
        size_t  cnt = 0;
        for(const Attribute& a : attrs)
            if(!a.key.empty() && a.key[0] != '%')
                ++cnt;
        if(!cnt)
            return ;
        
        for(Attribute&a : attrs){
            if(a.key.empty() || a.key[0] == '%')
                continue;
            h << "<tr><th align=\"left\">\n";
            for(size_t i =0; i<d; ++i)
                h << "&ensp;";
            h << a.key << "</th><td>" << a.data.qString().toHtmlEscaped() << "</td></tr>\n";
            if(!a.attrs.empty())
                _attributes(h, a.attrs, d+1);
        }
    }
    
    void    attrsPanel(HtmlWriter&h, const Vector<Class>& classes, Leaf::Merge&m)
    {
        h << "<caption class=\"attrs\">Attributes</caption>";
        if(x_atom)
            h << "<tr><th class=\"attrs\">Classes:</th><td>" << classes << "</td></tr>\n";
        _attributes(h, m.attrs);
    }


    void    fancyLeaf(HtmlWriter&h, const String& s)
    {
        test(determine_leaf(s));
        x_atom      = cdb::atom(x_leaf);
        Vector<Class>   classes = cdb::classes(x_atom);
        Set<Class>      cset    = makeSet(classes);
        classes.sort([](Class a, Class b) -> bool {
            return is_less(compare_igCase(cdb::name(a), cdb::name(b)));
        });

        
        
        //bool        isCharacter = cset.has(cdb::class_("Character"));
        //bool        isGroup     = cset.has(cdb::class_("Group"));
        //bool        isEvent     = cset.has(cdb::class_("Event"));
        

        auto m = cdb::merged(x_leaf);
        h.title(m->title());
        
        
        bool    mob = is_mobile();
        if(mob) 
            h << "<table class=\"attrs_mobile\">\n";
        else
            h << "<table class=\"attrs\" align=\"right\">\n";
//        h << "<tr><th>Name/Title</th><td>" << cdb::name(x_leaf) << "</td><tr>\n";
        
        attrsPanel(h, classes, *m);
        
        h << "</table>\n";
        
        if(m->context.empty()){
            h << "<p>(No context provided)</p>\n";
        } else {
            for(const Root* rt : wksp::roots()){
                auto i = m->context.find(rt);
                if(i == m->context.end())
                    continue;
                
                h << H3(rt->name());
                switch(i->second.format){
                case Format::Markdown:
                    h << Mark( i->second.data.qBytes());
                    break;
                case Format::ASCII:
                case Format::Plain:
                    h << Pre( i->second.data.qBytes());
                    break;
                case Format::Html:
                    h << i->second.data;
                    break;
                default:
                    break;
                }
            }
        }
        
    }

    INVOKE(
        reg_dispatcher(hGet, "w", fancyLeaf);
    )
}

