////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/meta/MetaHtml.hpp>
#include <yq/meta/ArgMeta.hpp>
#include <yq/meta/MethodMeta.hpp>
#include <yq/meta/OperatorMeta.hpp>
#include <yq/meta/ObjectMeta.hpp>
#include <yq/meta/PropertyMeta.hpp>
#include <yq/meta/TypeMeta.hpp>

#include <yq/mithril/web/WebContext.hpp>
#include <yq/mithril/web/WebHtml.hpp>
#include <yq/mithril/web/WebPage.hpp>
#include <yq/mithril/web/WebVariable.hpp>

namespace yq::mithril {

////////////////////////////////////////////////////////////////////////////////
//  META
////////////////////////////////////////////////////////////////////////////////

    namespace html {
        WebHtml&    operator<<(WebHtml&h, Dev<const MetaBase*>v)
        {
            if(v.data){
                h << v.data->name();
                //h << "<a href=\"/dev/meta/object?id=" << v.data->id() << "\">" << v.data->name() << "</a>";
            } else {
                h << "(null meta-info)";
            }
            return h;
        }

        WebHtml&    operator<<(WebHtml&h, DevID<const MetaBase*>v)
        {
            if(v.data){
                h << v.data->id();
                //h << "<a href=\"/dev/meta/object?id=" << v.data->id() << "\">" << v.data->id() << "</a>";
            } else {
                h << "(null meta)";
            }
            return h;
        }
    }

////////////////////////////////////////////////////////////////////////////////
//  METHODS
////////////////////////////////////////////////////////////////////////////////

    namespace html {
        WebHtml&    operator<<(WebHtml&h, Dev<const MethodMeta*> v)
        {
            if(v.data){
                h << v.data->name();
                //h << "<a href=\"/dev/meta/object?id=" << v.data->id() << "\">" << v.data->name() << "</a>";
            } else {
                h << "(null method-info)";
            }
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, DevID<const MethodMeta*> v)
        {
            if(v.data){
                h << v.data->id();
                //h << "<a href=\"/dev/meta/object?id=" << v.data->id() << "\">" << v.data->id() << "</a>";
            } else {
                h << "(null method-info)";
            }
            return h;
        }
        
        void    dev_table(WebHtml&h, const std::vector<const MethodMeta*>& methods)
        {
            auto ta = h.table();
            h << "<tr><th>ID</th><th>Name</th><th>Result</th><th>Const</th><th>Static</th><th>Description</th></tr>\n";
            for(const MethodMeta* mi : methods){
                h << "<tr><td>"  << dev_id(mi) 
                  << "</td><td>" << dev(mi) 
                  << "</td><td>";
                  
                if(mi->result())
                    h << dev(&mi->result()->type());
                    
                h << "</td><td>" << mi->is_const() 
                  << "</td><td>" << mi->is_static() 
                  << "</td><td>" << mi->description() 
                  << "</td></tr>\n";
            }
        }
    }
    
////////////////////////////////////////////////////////////////////////////////
//  OPERATORS
////////////////////////////////////////////////////////////////////////////////

    namespace html {
        WebHtml&    operator<<(WebHtml&h, Dev<const OperatorMeta*> v)
        {
            if(v.data){
                h << v.data->name();
                //h << "<a href=\"/dev/meta/object?id=" << v.data->id() << "\">" << v.data->name() << "</a>";
            } else {
                h << "(null operator-info)";
            }
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, DevID<const OperatorMeta*> v)
        {
            if(v.data){
                h << v.data->id();
                //h << "<a href=\"/dev/meta/object?id=" << v.data->id() << "\">" << v.data->id() << "</a>";
            } else {
                h << "(null operator-info)";
            }
            return h;
        }
        
        void    dev_table(WebHtml&h, const std::vector<const OperatorMeta*>& operators)
        {
            auto ta = h.table();
            h << "<tr><th>ID</th><th>Operator</th><th>Result</th><th>Const</th><th>Static</th><th>Description</th></tr>\n";
            for(const OperatorMeta* mi : operators){
                h << "<tr><td>"  << dev_id(mi) 
                  << "</td><td>" << key(mi->code())
                  << "</td><td>";
                  
                if(mi->result())
                    h << dev(&mi->result()->type());
                    
                h << "</td><td>" << mi->is_const() 
                  << "</td><td>" << mi->is_static() 
                  << "</td><td>" << mi->description() 
                  << "</td></tr>\n";
            }
        }
    }

////////////////////////////////////////////////////////////////////////////////
//  OBJECTS
////////////////////////////////////////////////////////////////////////////////
    
    namespace html {
        WebHtml&    operator<<(WebHtml&h, Dev<const ObjectMeta*> v)
        {
            if(v.data){
                h << "<a href=\"/dev/meta/object?id=" << v.data->id() << "\">" << v.data->name() << "</a>";
            } else {
                h << "(null object-info)";
            }
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, DevID<const ObjectMeta*> v)
        {
            if(v.data){
                h << "<a href=\"/dev/meta/object?id=" << v.data->id() << "\">" << v.data->id() << "</a>";
            } else {
                h << "(null object-info)"; 
            }
            return h;
        }
        
        void    dev_table(WebHtml&h, const std::vector<const ObjectMeta*>& objects)
        {
            auto ta = h.table();
            h << "<tr><th>ID</th><th>Name</th><th>Base</th><th>Size</th><th>Description</th></tr>\n";
            for(const ObjectMeta* ti : objects){
                h << "<tr><td>"  << dev_id(ti) 
                  << "</td><td>" << dev(ti) 
                  << "</td><td>" << dev(ti->base()) 
                  << "</td><td>" << ti->size() 
                  << "</td><td>" << ti->description() 
                  << "</td></tr>\n";
            }
        }
    }

////////////////////////////////////////////////////////////////////////////////
//  PROPERTIES
////////////////////////////////////////////////////////////////////////////////

    namespace html {
        WebHtml&    operator<<(WebHtml&h, Dev<const PropertyMeta*> v)
        {
            if(v.data){
                h << v.data->name();
                //h << "<a href=\"/dev/meta/object?id=" << v.data->id() << "\">" << v.data->name() << "</a>";
            } else {
                h << "(null property-info)";
            }
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, DevID<const PropertyMeta*> v)
        {
            if(v.data){
                h << v.data->id();
                //h << "<a href=\"/dev/meta/object?id=" << v.data->id() << "\">" << v.data->id() << "</a>";
            } else {
                h << "(null property-info)";
            }
            return h;
        }
        
        void    dev_table(WebHtml&h, const std::vector<const PropertyMeta*>& objects)
        {
            auto ta = h.table();
            h << "<tr><th>ID</th><th>Name</th><th>Type</th><th>State</th><th>Static</th><th>Description</th></tr>\n";
            for(const PropertyMeta* ti : objects){
                h << "<tr><td>"  << dev_id(ti) 
                  << "</td><td>" << dev(ti) 
                  << "</td><td>" << dev(&ti->type()) 
                  << "</td><td>" << ti->is_state() 
                  << "</td><td>" << ti->is_static() 
                  << "</td><td>" << ti->description() 
                  << "</td></tr>\n";
            }
        }
    }

////////////////////////////////////////////////////////////////////////////////
//  TYPE INFO
////////////////////////////////////////////////////////////////////////////////

    namespace html {
        WebHtml&    operator<<(WebHtml&h, Dev<const TypeMeta*> v)
        {
            if(v.data){
                h << "<a href=\"/dev/meta/type?id=" << v.data->id() << "\">" << v.data->name() << "</a>";
            } else {
                h << "(null type info)";
            }
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, DevID<const TypeMeta*> v)
        {
            if(v.data){
                h << "<a href=\"/dev/meta/type?id=" << v.data->id() << "\">" << v.data->id() << "</a>";
            } else {
                h << "(null type info)";
            }
            return h;
        }
        
        void    dev_table(WebHtml&h, const std::vector<const TypeMeta*>& types)
        {
            auto ta = h.table();
            ta.row().header("ID").header("Name").header("Size").header("Description").header("Stream?").header("Print?").header("Parse?");
            for(const TypeMeta* ti : types){
                if(!ti)
                    continue;
                
                auto tr = ta.row();
                tr.cell() << dev_id(ti);
                tr.cell() << dev(ti);
                tr.cell() << ti->size();
                tr.cell() << ti->description();
            }
        }
    }

////////////////////////////////////////////////////////////////////////////////
//  WEB STUFF
////////////////////////////////////////////////////////////////////////////////

    WebActionMethodPageMap  remapToActionMethods(const WebPageMap&wm)
    {
        WebActionMethodPageMap  ret;
        for(HttpOp h : HttpOp::all_values()){
            for(auto& itr : wm[h])
                ret[itr.first][h] = itr.second;
        }
        return ret;
    }

    namespace html {
        void    dev_table(WebHtml& out, const WebPageMap& wm, bool links)
        {
            auto    t   = out.table();
            for(auto& i : remapToActionMethods(wm)){
                const WebPage*get = nullptr;
                out << "<TR><TH align=\"left\">" << i.first << "</TH>";
                for(HttpOp h : HttpOp::all_values()){
                    out << "<TD>";
                    const WebPage*pg = i.second[h];
                    if(pg){
                        if(h == hGet)
                            get = pg;
                        if(links && (h == hGet)){
                            out << "<a href=\"" << i.first << "\">" << h << "</a>";
                        } else 
                            out << h;
                        if(pg->local_only())
                            out << "<br><i>local</i>";
                            
                        //  Only needed when troubleshooting
                        //out << "<br>" << pg->source().file_name() << ":" << pg->source().line();
                    }
                    out << "</TD>";
                }
                out << "<TD>";
                if(get){
                    out << get->description();
                }
                out << "</TD>";
                out << "</TR>\n";
            }
        } 

        void    dev_table(WebHtml& out, const WebVarMap& wm)
        {
            auto    t   = out.table();
            for(auto& i : wm){
                out << "<TR><TH align=\"left\">" << i.first << "</TH><TD>" << i.second->description() << "</TD></TR>\n";
            }
        }
    }
}
