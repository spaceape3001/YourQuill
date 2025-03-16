////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/meta/MetaHtml.hpp>
#include <yq/meta/ArgInfo.hpp>
#include <yq/meta/MethodInfo.hpp>
#include <yq/meta/OperatorInfo.hpp>
#include <yq/meta/ObjectInfo.hpp>
#include <yq/meta/PropertyInfo.hpp>
#include <yq/meta/TypeInfo.hpp>

#include <mithril/web/WebContext.hpp>
#include <mithril/web/WebHtml.hpp>
#include <mithril/web/WebPage.hpp>
#include <mithril/web/WebVariable.hpp>

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
        WebHtml&    operator<<(WebHtml&h, Dev<const MethodInfo*> v)
        {
            if(v.data){
                h << v.data->name();
                //h << "<a href=\"/dev/meta/object?id=" << v.data->id() << "\">" << v.data->name() << "</a>";
            } else {
                h << "(null method-info)";
            }
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, DevID<const MethodInfo*> v)
        {
            if(v.data){
                h << v.data->id();
                //h << "<a href=\"/dev/meta/object?id=" << v.data->id() << "\">" << v.data->id() << "</a>";
            } else {
                h << "(null method-info)";
            }
            return h;
        }
        
        void    dev_table(WebHtml&h, const std::vector<const MethodInfo*>& methods)
        {
            auto ta = h.table();
            h << "<tr><th>ID</th><th>Name</th><th>Result</th><th>Const</th><th>Static</th><th>Description</th></tr>\n";
            for(const MethodInfo* mi : methods){
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
        WebHtml&    operator<<(WebHtml&h, Dev<const OperatorInfo*> v)
        {
            if(v.data){
                h << v.data->name();
                //h << "<a href=\"/dev/meta/object?id=" << v.data->id() << "\">" << v.data->name() << "</a>";
            } else {
                h << "(null operator-info)";
            }
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, DevID<const OperatorInfo*> v)
        {
            if(v.data){
                h << v.data->id();
                //h << "<a href=\"/dev/meta/object?id=" << v.data->id() << "\">" << v.data->id() << "</a>";
            } else {
                h << "(null operator-info)";
            }
            return h;
        }
        
        void    dev_table(WebHtml&h, const std::vector<const OperatorInfo*>& operators)
        {
            auto ta = h.table();
            h << "<tr><th>ID</th><th>Operator</th><th>Result</th><th>Const</th><th>Static</th><th>Description</th></tr>\n";
            for(const OperatorInfo* mi : operators){
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
        WebHtml&    operator<<(WebHtml&h, Dev<const ObjectInfo*> v)
        {
            if(v.data){
                h << "<a href=\"/dev/meta/object?id=" << v.data->id() << "\">" << v.data->name() << "</a>";
            } else {
                h << "(null object-info)";
            }
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, DevID<const ObjectInfo*> v)
        {
            if(v.data){
                h << "<a href=\"/dev/meta/object?id=" << v.data->id() << "\">" << v.data->id() << "</a>";
            } else {
                h << "(null object-info)"; 
            }
            return h;
        }
        
        void    dev_table(WebHtml&h, const std::vector<const ObjectInfo*>& objects)
        {
            auto ta = h.table();
            h << "<tr><th>ID</th><th>Name</th><th>Base</th><th>Size</th><th>Description</th></tr>\n";
            for(const ObjectInfo* ti : objects){
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
        WebHtml&    operator<<(WebHtml&h, Dev<const PropertyInfo*> v)
        {
            if(v.data){
                h << v.data->name();
                //h << "<a href=\"/dev/meta/object?id=" << v.data->id() << "\">" << v.data->name() << "</a>";
            } else {
                h << "(null property-info)";
            }
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, DevID<const PropertyInfo*> v)
        {
            if(v.data){
                h << v.data->id();
                //h << "<a href=\"/dev/meta/object?id=" << v.data->id() << "\">" << v.data->id() << "</a>";
            } else {
                h << "(null property-info)";
            }
            return h;
        }
        
        void    dev_table(WebHtml&h, const std::vector<const PropertyInfo*>& objects)
        {
            auto ta = h.table();
            h << "<tr><th>ID</th><th>Name</th><th>Type</th><th>State</th><th>Static</th><th>Description</th></tr>\n";
            for(const PropertyInfo* ti : objects){
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
        WebHtml&    operator<<(WebHtml&h, Dev<const TypeInfo*> v)
        {
            if(v.data){
                h << "<a href=\"/dev/meta/type?id=" << v.data->id() << "\">" << v.data->name() << "</a>";
            } else {
                h << "(null type info)";
            }
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, DevID<const TypeInfo*> v)
        {
            if(v.data){
                h << "<a href=\"/dev/meta/type?id=" << v.data->id() << "\">" << v.data->id() << "</a>";
            } else {
                h << "(null type info)";
            }
            return h;
        }
        
        void    dev_table(WebHtml&h, const std::vector<const TypeInfo*>& types)
        {
            auto ta = h.table();
            ta.row().header("ID").header("Name").header("Size").header("Description").header("Stream?").header("Print?").header("Parse?");
            for(const TypeInfo* ti : types){
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
