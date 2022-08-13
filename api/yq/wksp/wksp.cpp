////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

//#include "Authentication.hpp"
//#include "AuthAccept.hpp"
//#include "AuthReject.hpp"
//#include "AuthSimple.hpp"

#include <kernel/user/UserCDB.hpp>
#include <kernel/user/UserFile.hpp>

#include "UserArg.hpp"
#include "UserHtml.hpp"
#include "UserPost.hpp"

#include <kernel/bit/KeyValue.hpp>
#include <basic/CollectionUtils.hpp>
#include <yq/db/HtmlLayout.hpp>
#include <kernel/db/SQ.hpp>
#include <kernel/file/DocumentCDB.hpp>
#include <yq/file/DocumentHtml.hpp>
#include <kernel/file/FolderCDB.hpp>
#include <kernel/file/FragmentCDB.hpp>
#include <kernel/file/Root.hpp>
#include <yq/file/RootHtml.hpp>
#include <kernel/image/ImageCDB.hpp>
#include <kernel/io/Strings.hpp>
#include <basic/StreamOps.hpp>
#include <basic/TextUtils.hpp>
#include <yq/web/WebContext.hpp>
#include <yq/web/WebHtml.hpp>
#include <kernel/wksp/Workspace.hpp>

#include <kernel/db/IDLock.ipp>


namespace yq {


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////




    
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////

    namespace arg {
        User user(std::string_view  arg_string)
        {
            arg_string   = trimmed(arg_string);
            if(arg_string.empty())
                return User{};
                
            User t   = cdb::user( arg_string);
            if(t)
                return t;
            uint64_t    i = to_uint64( arg_string).value;
            if(cdb::exists_user(i))
                return User{i};
            return User{};
        }
        
        User user(const WebContext&ctx, bool *detected)
        {
            if(detected)
                *detected = false;
                
            std::string    k    = ctx.find_query("id");
            if(!k.empty()){
                if(detected)
                    *detected = true;
                return user_id(k);
            }
            
            k       = ctx.find_query("key");
            if(!k.empty()){
                if(detected)
                    *detected = true;
                return user_key(k);
            }
            
            k       = ctx.find_query("user");
            if(!k.empty()){
                if(detected)
                    *detected = true;
                return user(k);
            }
            return User{};
        }
        
        User user(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return user(arg_string);
        }
        
        User user(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return user(arg_string);
        }
        

        User user_id(std::string_view arg_string)
        {
            uint64_t    i   = to_uint64(arg_string).value;
            if(cdb::exists_user(i))
                return User{i};
            return User{};
        }

        User user_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return user_id(arg_string);
        }
        
        User user_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return user_id(arg_string);
        }
        
        User user_key(std::string_view arg_string)
        {
            return cdb::user(trimmed(arg_string));
        }
        
        User user_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return user_key(arg_string);
        }
        
        User user_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return user_key(arg_string);
        }
    }

////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////

    namespace html {

        WebHtml&    operator<<(WebHtml&h, User v)
        {
            Thumbnail th = cdb::thumbnail(cdb::icon(v), h.context().session.icon_size);
            
            //  start the url (later)
            if(th)
                h << th << " ";
            
            h << cdb::label(v);
            //  end the url (later)
            return h;
        }

        WebHtml&    operator<<(WebHtml&h, Dev<User> v)
        {
            std::string n       = cdb::name(v.data);
            if(n.empty())
                n   = "(no-name)";
            if(v.data)
                h << "<a href=\"/dev/user?id=" << v.data.id << "\">";
            h << "[" << v.data.id << "] " << n;
            if(v.data)
                h << "</a>";
            return h;
        }

        WebHtml&    operator<<(WebHtml&h, DevID<User> v)
        {
            if(v.data)
                h << "<a href=\"/dev/user?id=" << v.data.id << "\">";
            h << v.data.id;
            if(v.data)
                h << "</a>";
            return h;
        }

        void    admin_table(WebHtml&h, const std::vector<User>&users)
        {
            auto tac = h.table();
            auto iz = h.context().session.icon_size;
            html::columns(h, users, 
                [&](User c)
                {
                    if(c){
                        Image   i   = cdb::icon(c);
                        if(i){
                            h << cdb::thumbnail(i, iz);
                        } else
                            h << "<img src=\"/img/generic.svg\" class=\"" << iz << "\">";
                    }
                },
                [&](User c)
                {
                    if(c){
                        h << "<a href=\"/admin/user?id=" << c.id << "\">" << cdb::label(c) << "</a>";
                    }
                }
            );
        }

        void    dev_table(WebHtml& h, const std::vector<User>&users)
        {
            auto ta = h.table();
            h << "<tr><th>ID</th><th>Key</th><th>Owner</th><th>Admin</th><th>Writer</th><th>Reader</th><th>Name</th><th>Doc</th><th>Brief</th></tr>\n";
            for(User t : users){
                auto i  = cdb::info(t);
                h << "<tr><td>" << dev_id(t) << "</td><td>" << i.key << "</td><td>" 
                    << i.is_owner << "</td><td>"
                    << i.is_admin << "</td><td>"
                    << i.is_writer << "</td><td>"
                    << i.is_reader << "</td><td>"
                    <<  i.name << "</td><td>" << dev(i.doc) << "</td><td>" << i.brief << "</td></tr>\n";
            }
        }    

        void        new_user_control(WebHtml&h, std::string_view npath)
        {
            Url url;
            url.path=copy(npath);
            h << html::form_start(url, false);
            h << "Add User:<br>";
            h << ikey();
            h << "<br><hr width=\"10%\">\n";
            h << iroot( DataRole::Users );
            h << "<hr width=\"10%\">\n";
            h << iedit();
            h << "<hr width=\"10%\">\n";
            h << Submit(Submit::Create);
            h << "</form>\n";
        }

    }

////////////////////////////////////////////////////////////////////////////////

    namespace post {

        User user(WebContext&ctx, bool *detected)
        {
            ctx.decode_post();
            
            if(detected)
                *detected   = false;
                
            std::string    k    = ctx.rx_post.first("user");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return arg::user_id(k);
            }
            return User();
        }
        
        User user(WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_name));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::user_id(arg_string);
        }
        
        User user(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_names));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::user_id(arg_string);
        }
    }

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}

