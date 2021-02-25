////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "yNetWriter.hpp"
#include "yPage.hpp"
#include "yCommon.hpp"

#include <httpserver/httprequest.h>

#include "db/Cache.hpp"
#include "util/DelayInit.hpp"
#include "util/Utilities.hpp"

namespace {
    using namespace html;
    
    
    QString cls_title()
    {
        return QString("Class '%1'").arg(cdb::name(x_class));
    }
    
    void    _context(Html& h, const Leaf::Merge& m)
    {
        if(m.context.empty()){
            h << "(no context provided)";
        } else {
            for(const Root* rt : wksp::roots()){
                auto i = m.context.find(rt);
                if(i == m.context.end())
                    continue;
                
                h << H2(rt->name());
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
    
    void    _attributes(Html& h, const Vector<Attribute>& vdata)
    {
        if(vdata.empty())
            return ;
            
        h << "<ul>\n";
        for(auto& a : vdata){
            h << "<li> <b>" << a.key << "</b> &mdash; " << a.data << "\n";
            _attributes(h, a.attrs);
        }
        h << "</ul>\n";
    }

    void    page_atom(Html& h)
    {
        test(decode_atom_prime(), false);
        x_leaf      = cdb::leaf(x_atom);
        x_document  = cdb::document(x_atom);
        bool        is  = x_leaf;
        if(!is)
            x_leaf  = cdb::leaf(x_document.id);

        auto        m  = cdb::merged(x_leaf);
        h.title(cdb::title(x_atom));
        
        h << H1("Context");
        if(is){
            _context(h, *m);
        } else {
            h << "(Not a leaf, no context available.)";
        }
    }
    
    
    void    page_class(Html&h)
    {
        test(decode_class_prime(), false);
        h.title(cls_title());
            
        h << "Under construction";
    }

    void    page_leaf(Html&h)
    {
        test(decode_leaf_prime());
        auto m = cdb::merged(x_leaf);
        h.title(m->title());
        
        h << H1("Attributes");
        _attributes(h, m->attrs);
        
        h << H1("Context");
        _context(h, *m);
    }
    
    void    page_leaf_attributes(Html& h)
    {
        test(decode_leaf_prime());
        auto m = cdb::merged(x_leaf);
        h.title(m->title() + " (Attributes)");
    }
    
    void    page_leaf_context(Html& h)
    {
        test(decode_leaf_prime());
        auto m = cdb::merged(x_leaf);
        h.title(m->title() + " (Context)");
        
    }
    
    ContentType     page_graph(QByteArray& bytes)
    {
        test(decode_graph_prime());
        bytes       = cdb::svg(x_graph);
        return ContentType::svg;
    }
    
    
    void    reg_stuff()
    {
        page(hGet, "/atom", page_atom).description("Atom").id().key().label("Overview");
        page(hGet, "/class", page_class).description("Class").id().key().label("Overview");
        page(hGet, "/graph", page_graph).description("Graph").id().key();
    
        tabbar({
            page(hGet, "/leaf", page_leaf).description("Leaf").id().key().label("Overview"),
            page(hGet, "/leaf/attributes", page_leaf_attributes).description("Leaf Attributes").argument("id").label("Attributes"),
            page(hGet, "/leaf/context", page_leaf_context).description("Leaf Context").argument("id").label("Context")
        });
    }
    
    INVOKE(reg_stuff(); )
}
