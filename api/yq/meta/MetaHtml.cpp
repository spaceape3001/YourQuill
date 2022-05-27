////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/meta/MetaHtml.hpp>
#include <basic/meta/ObjectInfo.hpp>
#include <basic/meta/PropertyInfo.hpp>
#include <basic/meta/TypeInfo.hpp>
#include <basic/TextUtils.hpp>
#include <yq/web/WebContext.hpp>
#include <yq/web/WebHtml.hpp>
#include <yq/web/WebPage.hpp>
#include <yq/web/WebVariable.hpp>

namespace yq {
    namespace arg {

        const ObjectInfo* object_info(std::string_view  arg_string)
        {
            arg_string   = trimmed(arg_string);
            if(arg_string.empty())
                return nullptr;
                
            const ObjectInfo* t   = ObjectInfo::find(arg_string);
            if(t)
                return t;
            return ObjectInfo::find(to_uint( arg_string).value);
        }
        
        const ObjectInfo* object_info(const WebContext&ctx, bool *detected)
        {
            if(detected)
                *detected   = false;
                
            std::string    k    = ctx.find_query("id");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return object_info_id(k);
            }
            
            k       = ctx.find_query("key");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return object_info_key(k);
            }
            
            k       = ctx.find_query("object");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return object_info(k);
            }
            return nullptr;
        }
        
        const ObjectInfo* object_info(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return object_info(arg_string);
        }
        
        const ObjectInfo* object_info(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return object_info(arg_string);
        }

        const ObjectInfo* object_info_id(std::string_view arg_string)
        {
            return ObjectInfo::find(to_uint(arg_string).value);
        }

        const ObjectInfo* object_info_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return object_info_id(arg_string);
        }
        
        const ObjectInfo* object_info_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return object_info_id(arg_string);
        }
        
        const ObjectInfo* object_info_key(std::string_view arg_string)
        {
            return ObjectInfo::find(trimmed(arg_string));
        }
        
        const ObjectInfo* object_info_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return object_info_key(arg_string);
        }
        
        const ObjectInfo* object_info_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return object_info_key(arg_string);
        }
    }

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

    namespace arg {
        const TypeInfo* type_info(std::string_view  arg_string)
        {
            arg_string   = trimmed(arg_string);
            if(arg_string.empty())
                return nullptr;
                
            const TypeInfo* t   = TypeInfo::find(arg_string);
            if(t)
                return t;
            return TypeInfo::find(to_uint( arg_string).value);
        }
        
        const TypeInfo* type_info(const WebContext&ctx, bool *detected)
        {
            if(detected)
                *detected   = false;
                
            std::string    k    = ctx.find_query("id");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return type_info_id(k);
            }
            
            k       = ctx.find_query("key");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return type_info_key(k);
            }
            
            k       = ctx.find_query("type");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return type_info(k);
            }
            return nullptr;
        }
        
        const TypeInfo* type_info(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return type_info(arg_string);
        }
        
        const TypeInfo* type_info(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return type_info(arg_string);
        }

        const TypeInfo* type_info_id(std::string_view arg_string)
        {
            return TypeInfo::find(to_uint(arg_string).value);
        }

        const TypeInfo* type_info_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return type_info_id(arg_string);
        }
        
        const TypeInfo* type_info_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return type_info_id(arg_string);
        }
        
        const TypeInfo* type_info_key(std::string_view arg_string)
        {
            return TypeInfo::find(trimmed(arg_string));
        }
        
        const TypeInfo* type_info_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return type_info_key(arg_string);
        }
        
        const TypeInfo* type_info_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return type_info_key(arg_string);
        }
    }

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
            h << "<tr><th>ID</th><th>Name</th><th>Size</th><th>Description</th></tr>\n";
            for(const TypeInfo* ti : types){
                if(!ti)
                    continue;
                h << "<tr><td>"  << dev_id(ti) 
                  << "</td><td>" << dev(ti) 
                  << "</td><td>" << ti->size() 
                  << "</td><td>" << ti->description() 
                  << "</td></tr>\n";
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
                out << "<TR><TH align=\"left\">" << i.first << "</TH>";
                for(HttpOp h : HttpOp::all_values()){
                    out << "<TD>";
                    const WebPage*pg = i.second[h];
                    if(pg){
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
