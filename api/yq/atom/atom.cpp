////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <kernel/atom/AtomCDB.hpp>

#include "AtomArg.hpp"
#include "AtomHtml.hpp"
#include "AtomPost.hpp"

#include <kernel/atom/ClassCDB.hpp>
#include <kernel/atom/ClassFile.hpp>
#include "ClassArg.hpp"
#include "ClassHtml.hpp"
#include "ClassPost.hpp"

#include <kernel/atom/FieldCDB.hpp>
#include <kernel/atom/FieldFile.hpp>
#include "FieldArg.hpp"
#include "FieldHtml.hpp"
#include "FieldPost.hpp"

#include <kernel/atom/ValueCDB.hpp>
#include <kernel/atom/ValueFile.hpp>
#include "ValueHtml.hpp"

#include <kernel/bit/KeyValue.hpp>
#include <basic/CollectionUtils.hpp>
#include <kernel/db/CacheFwd.hpp>
#include <yq/db/HtmlLayout.hpp>
#include <kernel/db/NKI.hpp>
#include <kernel/file/DocumentCDB.hpp>
#include <yq/file/DocumentHtml.hpp>
#include <kernel/file/FolderCDB.hpp>
#include <kernel/file/FragmentCDB.hpp>
#include <kernel/file/Root.hpp>
#include <yq/file/RootHtml.hpp>
#include <kernel/image/ImageCDB.hpp>
#include <yq/image/ImageHtml.hpp>
#include <basic/FileUtils.hpp>
#include <kernel/io/Strings.hpp>
#include <basic/XmlUtils.hpp>
#include <basic/meta/TypeInfo.hpp>
#include <kernel/org/CategoryCDB.hpp>
#include <yq/org/TagHtml.hpp>
#include <kernel/org/TagCDB.hpp>
#include <basic/StreamOps.hpp>
#include <basic/stream/Text.hpp>
#include <basic/TextUtils.hpp>
#include <yq/web/WebContext.hpp>
#include <yq/web/WebHtml.hpp>
#include <kernel/wksp/Workspace.hpp>


namespace yq {

////////////////////////////////////////////////////////////////////////////////
//  (ATOM)
////////////////////////////////////////////////////////////////////////////////

    namespace arg {
        Atom atom(std::string_view  arg_string)
        {
            arg_string   = trimmed(arg_string);
            if(arg_string.empty())
                return Atom{};
                
            Atom t   = cdb::atom( arg_string);
            if(t)
                return t;
            uint64_t    i = to_uint64( arg_string).value;
            if(cdb::exists_atom(i))
                return Atom{i};
            return Atom{};
        }
        
        Atom atom(const WebContext&ctx, bool *detected)
        {
            std::string    k    = ctx.find_query("id");
            if(!k.empty())
                return atom_id(k);
            
            k       = ctx.find_query("key");
            if(!k.empty())
                return atom_key(k);
            
            k       = ctx.find_query("atom");
            if(!k.empty())
                return atom(k);
            return Atom{};
        }
        
        
        Atom atom(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return atom(arg_string);
        }
        
        Atom atom(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return atom(arg_string);
        }

        Atom atom_id(std::string_view arg_string)
        {
            uint64_t    i   = to_uint64(arg_string).value;
            if(cdb::exists_atom(i))
                return Atom{i};
            return Atom{};
        }

        Atom atom_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return atom_id(arg_string);
        }
        
        Atom atom_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return atom_id(arg_string);
        }
        
        Atom atom_key(std::string_view arg_string)
        {
            return cdb::atom(trimmed(arg_string));
        }
        
        Atom atom_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return atom_key(arg_string);
        }
        
        Atom atom_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return atom_key(arg_string);
        }
    }
    
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////

    namespace html {
        WebHtml&    operator<<(WebHtml&h, Atom v)
        {
            //  more TODO...
            h << cdb::label(v);
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, Dev<Atom> v)
        {
            if(v.data)
                h << "<a href=\"/dev/atom?id=" << v.data.id << "\">";
            h << cdb::label(v.data);
            if(v.data)
                h << "</a>";
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, DevID<Atom> v)
        {
            if(v.data)
                h << "<a href=\"/dev/atom?id=" << v.data.id << "\">";
            h << v.data.id;
            if(v.data)
                h << "</a>";
            return h;
        }
        
        void        dev_table(WebHtml&h, const std::vector<Atom>& atoms)
        {
            auto t = h.table();
            h << "<tr><th>ID</th><th>Key</th><th>Brief</th></tr>\n";
            for(Atom a : atoms){
                auto i = cdb::info(a);
                h << "<tr><td>" << dev_id(a) << "</td><td>" << i.key << "</td><td>" << i.brief << "</td></tr>\n";
            }
        }
    }

////////////////////////////////////////////////////////////////////////////////

    namespace post {

        Atom atom(WebContext&ctx, bool *detected)
        {
            ctx.decode_post();
            
            if(detected)
                *detected   = false;
                
            std::string    k    = ctx.rx_post.first("atom");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return arg::atom_id(k);
            }
            return Atom();
        }
        
        Atom atom(WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_name));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::atom_id(arg_string);
        }
        
        Atom atom(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_names));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::atom_id(arg_string);
        }
    }

////////////////////////////////////////////////////////////////////////////////
//  (ATOM) CLASS(IFICATIONS)
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////


    namespace arg {
        Class class_(std::string_view  arg_string)
        {
            arg_string   = trimmed(arg_string);
            if(arg_string.empty())
                return Class{};
                
            Class t   = cdb::class_( arg_string);
            if(t)
                return t;
            uint64_t    i = to_uint64( arg_string).value;
            if(cdb::exists_class(i))
                return Class{i};
            return Class{};
        }
        
        Class class_(const WebContext&ctx, bool *detected)
        {
            std::string    k    = ctx.find_query("id");
            if(!k.empty())
                return class_id(k);
            
            k       = ctx.find_query("key");
            if(!k.empty())
                return class_key(k);
            
            k       = ctx.find_query("class");
            if(!k.empty())
                return class_(k);
            return Class{};
        }
        
        
        Class class_(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return class_(arg_string);
        }
        
        Class class_(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return class_(arg_string);
        }

        Class class_id(std::string_view arg_string)
        {
            uint64_t    i   = to_uint64(arg_string).value;
            if(cdb::exists_class(i))
                return Class{i};
            return Class{};
        }

        Class class_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return class_id(arg_string);
        }
        
        Class class_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return class_id(arg_string);
        }
        
        Class class_key(std::string_view arg_string)
        {
            return cdb::class_(trimmed(arg_string));
        }
        
        Class class_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return class_key(arg_string);
        }
        
        Class class_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return class_key(arg_string);
        }
    }

////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////

    namespace html {
        WebHtml&    operator<<(WebHtml&h, Class v)
        {
            Thumbnail th = cdb::thumbnail(cdb::icon(v), h.context().session.icon_size);
            if(v)
                h << "<a href=\"/class?key=" << cdb::key(v) << "\">";
            //  start the url (later)
            if(th)
                h << th << " ";
            h << cdb::label(v);
            if(v)
                h << "</a>";
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, Dev<Class> v)
        {
            if(v.data)
                h << "<a href=\"/dev/class?id=" << v.data.id << "\">";
            h << cdb::label(v.data);
            if(v.data)
                h << "</a>";
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, DevID<Class> v)
        {
            if(v.data)
                h << "<a href=\"/dev/class?id=" << v.data.id << "\">";
            h << v.data.id;
            if(v.data)
                h << "</a>";
            return h;
        }

        WebHtml&    operator<<(WebHtml&h, const Edit<Class>& v)
        {
            Url             url;
            url.path        = "/admin/class/edit";
            const Root* rt  = v.root;
            if(!rt)
                rt          = h.context().def_root(DataRole::Config);

            Class   c   = v.value;
            if(!c)
                c       = arg::class_(h);
            
            {
                stream::Text    args(url.query);
                if(c)
                    args << "id=" << c.id;
                if(rt){
                    if(c)
                        args << '&';
                    args << "root=" << rt->id;
                }
            }
            
            h << form_start(url, v.force_inspect);
            return h;
        }

        WebHtml&    operator<<(WebHtml&h, const Plural<Class>& v)
        {
            Thumbnail th = cdb::thumbnail(cdb::icon(v.data), h.context().session.icon_size);
            if(v.data)
                h << "<a href=\"/atoms?class=" << cdb::key(v.data) << "\">";
            //  start the url (later)
            if(th)
                h << th << " ";
            h << cdb::label(v.data);
            if(v.data)
                h << "</a>";
            return h;
        }

        void        admin_table(WebHtml&h, const std::vector<Class>&classes)
        {
            auto tac = h.table();
            auto iz = h.context().session.icon_size;
            html::columns(h, classes, 
                [&](Class c)
                {
                    if(c){
                        Image   i   = cdb::icon(c);
                        if(i){
                            h << cdb::thumbnail(i, iz);
                        } else
                            h << "<img src=\"/img/generic.svg\" class=\"" << iz << "\">";
                    }
                },
                [&](Class c)
                {
                    if(c){
                        h << "<a href=\"/admin/class?id=" << c.id << "\">" << cdb::label(c) << "</a>";
                    }
                }
            );
        }
        

        void        dev_table(WebHtml&h, const std::vector<Class>& classes)
        {
            auto ta = h.table();
            h << "<tr><th>ID</th><th>Key</th><th>Name</th><th>Doc</th><th>Brief</th></tr>\n";
            for(Class c : classes){
                auto i = cdb::info(c);
                h << "<tr><td>" << dev_id(c) << "</td><td>" << i.key << "</td><td>" << i.name << "</td><td>" 
                    << dev(i.doc) << "</td><td>" << i.brief << "</td></tr>\n";
            }
        }

        void        new_class_control(WebHtml&h, std::string_view npath)
        {
            Url url;
            url.path=copy(npath);
            h << html::form_start(url, false);
            h << "Add Class:<br>";
            h << ikey();
            h << "<br><hr width=\"10%\">\n";
            h << iroot( DataRole::Config, Access::ReadWrite );
            h << "<hr width=\"10%\">\n";
            h << iedit();
            h << "<hr width=\"10%\">\n";
            h << Submit(Submit::Create);
            h << "</form>\n";
        }
        
    }

////////////////////////////////////////////////////////////////////////////////

    namespace post {

        Class class_(WebContext&ctx, bool *detected)
        {
            ctx.decode_post();
            
            if(detected)
                *detected   = false;
                
            std::string    k    = ctx.rx_post.first("class");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return arg::class_id(k);
            }
            return Class();
        }
        
        Class class_(WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_name));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::class_id(arg_string);
        }
        
        Class class_(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_names));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::class_id(arg_string);
        }
    }

////////////////////////////////////////////////////////////////////////////////
//  (ATOM) FIELD DEFINITIONS
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////


    namespace arg {
        Field field(std::string_view  arg_string)
        {
            arg_string   = trimmed(arg_string);
            if(arg_string.empty())
                return Field{};
                
            Field t   = cdb::field( arg_string);
            if(t)
                return t;
            uint64_t    i = to_uint64( arg_string).value;
            if(cdb::exists_field(i))
                return Field{i};
            return Field{};
        }
        
        Field field(const WebContext&ctx, bool *detected)
        {
            std::string    k    = ctx.find_query("id");
            if(!k.empty())
                return field_id(k);
            
            k       = ctx.find_query("key");
            if(!k.empty())
                return field_key(k);
            
            k       = ctx.find_query("field");
            if(!k.empty())
                return field(k);
            return Field{};
        }
        
        
        Field field(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return field(arg_string);
        }
        
        Field field(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return field(arg_string);
        }

        Field field_id(std::string_view arg_string)
        {
            uint64_t    i   = to_uint64(arg_string).value;
            if(cdb::exists_field(i))
                return Field{i};
            return Field{};
        }

        Field field_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return field_id(arg_string);
        }
        
        Field field_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return field_id(arg_string);
        }
        
        Field field_key(std::string_view arg_string)
        {
            return cdb::field(trimmed(arg_string));
        }
        
        Field field_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return field_key(arg_string);
        }
        
        Field field_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return field_key(arg_string);
        }
    }


////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////

    namespace html {
        WebHtml&    operator<<(WebHtml&h, Field v)
        {
            Thumbnail th = cdb::thumbnail(cdb::icon(v), h.context().session.icon_size);
            
            //  start the url (later)
            if(th)
                h << th << " ";
            
            h << cdb::label(v);
            //  end the url (later)
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, Dev<Field> v)
        {
            if(v.data)
                h << "<a href=\"/dev/field?id=" << v.data.id << "\">";
            h << cdb::label(v.data);
            if(v.data)
                h << "</a>";
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, DevID<Field> v)
        {
            if(v.data)
                h << "<a href=\"/dev/field?id=" << v.data.id << "\">";
            h << v.data.id;
            if(v.data)
                h << "</a>";
            return h;
        }

        void        admin_table(WebHtml&h, const std::vector<Field>&fields)
        {
            auto tac = h.table();
            auto iz = h.context().session.icon_size;
            html::columns(h, fields, 
                [&](Field c)
                {
                    if(c){
                        Image   i   = cdb::icon(c);
                        if(i){
                            h << cdb::thumbnail(i, iz);
                        } else
                            h << "<img src=\"/img/generic.svg\" class=\"" << iz << "\">";
                    }
                },
                [&](Field c)
                {
                    if(c){
                        h << "<a href=\"/admin/class?id=" << c.id << "\">" << cdb::label(c) << "</a>";
                    }
                }
            );
        }
        
        void        dev_table(WebHtml&h, const std::vector<Field>& fields)
        {
            auto t = h.table();
            h << "<tr><th>ID</th><th>Class</th><th>Key</th><th>Name</th><th>Brief</th>\n";
            for(Field f : fields){
                auto i = cdb::info(f);
                h << "<tr><td>" << dev_id(f) << "</td><td>" << dev(i.class_) << "</td><td>" 
                    << i.key <<"</td><td>" << i.name << "</td><td>"
                    << i.brief <<"</td></tr>\n";
            }
        }

        void        new_field_control(WebHtml&h, std::string_view npath, Class c)
        {
            Url url;
            url.path=copy(npath);
            h << html::form_start(url, false);
            h << "Add Field:<br>";
            h << ikey();
            h << "<br><hr width=\"10%\">\n";
            h << iroot( DataRole::Config, Access::ReadWrite );
            h << "<hr width=\"10%\">\n";
            h << iedit();
            h << "<hr width=\"10%\">\n";
            h << Submit(Submit::Create);
            if(c){
                h << ihidden("class", to_string(c.id));
            }
            
            h << "</form>\n";
        }
    }

////////////////////////////////////////////////////////////////////////////////

    namespace post {

        Field field(WebContext&ctx, bool *detected)
        {
            ctx.decode_post();
            
            if(detected)
                *detected   = false;
                
            std::string    k    = ctx.rx_post.first("field");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return arg::field_id(k);
            }
            return Field();
        }
        
        Field field(WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_name));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::field_id(arg_string);
        }
        
        Field field(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_names));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::field_id(arg_string);
        }
    }

}
