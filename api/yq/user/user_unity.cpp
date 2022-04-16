////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "UserFile.hpp"
#include "UserGroupFile.hpp"
#include "arg.hpp"
#include "cdb.hpp"
#include <yq/file/cdb.hpp>
#include <yq/file/Strings.hpp>
#include <yq/file/XmlUtils.hpp>
#include <yq/log/Logging.hpp>
#include <yq/text/Utils.hpp>
#include <yq/type/ByteArray.hpp>
#include <yq/web/WebContext.hpp>
#include <yq/web/WebHtml.hpp>
#include <yq/wksp/CacheFwd.hpp>
#include <yq/wksp/CacheSQ.hpp>
#include <yq/wksp/Root.hpp>
#include <yq/wksp/Workspace.hpp>

namespace yq {
    namespace {
        std::string user_filename(std::string_view k)
        {
            std::string ret(k);
            ret += ".user";
            return ret;
        }

        std::string usergroup_filename(std::string_view k)
        {
            std::string ret(k);
            ret += ".ugrp";
            return ret;
        }
    }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


    User::Data&  User::Data::merge(const Data&b, bool fOverride)
    {
        set_if_empty(name, b.name, fOverride);
        bio += b.bio;
        groups += groups;
        return *this;
    }
    
    void  User::Data::reset()
    {
        *this   = Data();
    }
    

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    void  User::File::reset() 
    {
        Data::reset();
    }
    
    bool  User::File::read(const XmlDocument&doc, std::string_view fname) 
    {
        const XmlNode*  xn  = doc.first_node(szYQUser);
        if(!xn){
            yError() << "No appropriate root element!  In: '" << fname << "'";
            return false;
        }
        
        xn      = xn -> first_node(szUser);
        if(!xn){
            yError() << "No user! In: " << fname << "'";
            return false;
        }
        
        name            = read_child(xn, szName, x_string);
        authentication  = read_child(xn, szAuthentication, x_authentication);
        if(!skip_bio)
            bio         = read_children(xn, szContext, x_context);
        std::vector<std::string>    theGroups = read_children(xn, szGroup, x_sstring);
        groups          = StringSet(theGroups.begin(), theGroups.end());
        return true;
    }
    
    bool  User::File::write(XmlDocument&doc) const 
    {
        xml_start(doc);
        
        XmlNode*  xroot  = doc.allocate_node(rapidxml::node_element, szYQUser);
        doc.append_node(xroot);
        XmlNode*    xuser   = doc.allocate_node(rapidxml::node_element, szUser);
        xroot -> append_node(xuser);
        
        write_child(xuser, szUser, name);
        write_child(xuser, szAuthentication, authentication);
        write_children(xuser, szContext, bio);
        write_children(xuser, szGroup, groups);
        return true;
    }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


    UserGroup::Data&  UserGroup::Data::merge(const Data&b, bool fOverride)
    {
        set_if_empty(name, b.name, fOverride);
        bio += b.bio;
        return *this;
    }
    
    void  UserGroup::Data::reset()
    {
        *this   = Data();
    }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    void  UserGroup::File::reset() 
    {
        Data::reset();
    }
    
    bool  UserGroup::File::read(const XmlDocument&doc, std::string_view fname) 
    {
        const XmlNode*  xn  = doc.first_node(szYQUserGroup);
        if(!xn){
            yError() << "No appropriate root element!  In: '" << fname << "'";
            return false;
        }
        
        xn      = xn -> first_node(szGroup);
        if(!xn){
            yError() << "No group! In: " << fname << "'";
            return false;
        }
        
        name            = read_child(xn, szName, x_string);
        if(!skip_bio)
            bio         = read_children(xn, szContext, x_context);
        return true;
    }
    
    bool  UserGroup::File::write(XmlDocument&doc) const 
    {
        xml_start(doc);
        
        XmlNode*  xroot  = doc.allocate_node(rapidxml::node_element, szYQUserGroup);
        doc.append_node(xroot);
        XmlNode*    xuser   = doc.allocate_node(rapidxml::node_element, szGroup);
        xroot -> append_node(xuser);
        write_children(xuser, szContext, bio);
        return true;
    }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

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
        
        User user(const WebContext&ctx)
        {
            std::string    k    = ctx.find_query("id");
            if(!k.empty())
                return user_id(k);
            
            k       = ctx.find_query("key");
            if(!k.empty())
                return user_key(k);
            
            k       = ctx.find_query("user");
            if(!k.empty())
                return user(k);
            return User{};
        }
        
        User user(const WebHtml&h)
        {
            return user(h.context());
        }
        
        User user(const WebContext&ctx, std::string_view arg_name)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            return user(arg_string);
        }
        
        User user(const WebHtml&h, std::string_view arg_name)
        {
            return user(h.context(), arg_name);
        }
        
        User user(const WebContext& ctx, std::initializer_list<std::string_view> arg_names)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            return user(arg_string);
        }
        
        User user(const WebHtml&h, std::initializer_list<std::string_view> arg_names)
        {
            return user(h.context(), arg_names);
        }

        User user_id(std::string_view arg_string)
        {
            uint64_t    i   = to_uint64(arg_string).value;
            if(cdb::exists_user(i))
                return User{i};
            return User{};
        }

        User user_id(const WebContext&ctx, std::string_view arg_name)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            return user_id(arg_string);
        }
        
        User user_id(const WebHtml&h, std::string_view arg_name)
        {
            return user_id(h.context(), arg_name);
        }
        
        User user_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            return user_id(arg_string);
        }
        
        User user_id(const WebHtml&h, std::initializer_list<std::string_view> arg_names)
        {
            return user_id(h.context(), arg_names);
        }
        
        User user_key(std::string_view arg_string)
        {
            return cdb::user(trimmed(arg_string));
        }
        
        User user_key(const WebContext&ctx, std::string_view arg_name)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            return user_key(arg_string);
        }
        
        User user_key(const WebHtml&h, std::string_view arg_name)
        {
            return user_key(h.context(), arg_name);
        }
        
        User user_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            return user_key(arg_string);
        }
        
        User user_key(const WebHtml&h, std::initializer_list<std::string_view> arg_names)
        {
            return user_key(h.context(), arg_names);
        }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        UserGroup usergroup(std::string_view  arg_string)
        {
            arg_string   = trimmed(arg_string);
            if(arg_string.empty())
                return UserGroup{};
                
            UserGroup t   = cdb::usergroup( arg_string);
            if(t)
                return t;
            uint64_t    i = to_uint64( arg_string).value;
            if(cdb::exists_usergroup(i))
                return UserGroup{i};
            return UserGroup{};
        }
        
        UserGroup usergroup(const WebContext&ctx)
        {
            std::string    k    = ctx.find_query("id");
            if(!k.empty())
                return usergroup_id(k);
            
            k       = ctx.find_query("key");
            if(!k.empty())
                return usergroup_key(k);
            
            k       = ctx.find_query("usergroup");
            if(!k.empty())
                return usergroup(k);
            return UserGroup{};
        }
        
        UserGroup usergroup(const WebHtml&h)
        {
            return usergroup(h.context());
        }
        
        UserGroup usergroup(const WebContext&ctx, std::string_view arg_name)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            return usergroup(arg_string);
        }
        
        UserGroup usergroup(const WebHtml&h, std::string_view arg_name)
        {
            return usergroup(h.context(), arg_name);
        }
        
        UserGroup usergroup(const WebContext& ctx, std::initializer_list<std::string_view> arg_names)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            return usergroup(arg_string);
        }
        
        UserGroup usergroup(const WebHtml&h, std::initializer_list<std::string_view> arg_names)
        {
            return usergroup(h.context(), arg_names);
        }

        UserGroup usergroup_id(std::string_view arg_string)
        {
            uint64_t    i   = to_uint64(arg_string).value;
            if(cdb::exists_usergroup(i))
                return UserGroup{i};
            return UserGroup{};
        }

        UserGroup usergroup_id(const WebContext&ctx, std::string_view arg_name)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            return usergroup_id(arg_string);
        }
        
        UserGroup usergroup_id(const WebHtml&h, std::string_view arg_name)
        {
            return usergroup_id(h.context(), arg_name);
        }
        
        UserGroup usergroup_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            return usergroup_id(arg_string);
        }
        
        UserGroup usergroup_id(const WebHtml&h, std::initializer_list<std::string_view> arg_names)
        {
            return usergroup_id(h.context(), arg_names);
        }
        
        UserGroup usergroup_key(std::string_view arg_string)
        {
            return cdb::usergroup(trimmed(arg_string));
        }
        
        UserGroup usergroup_key(const WebContext&ctx, std::string_view arg_name)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            return usergroup_key(arg_string);
        }
        
        UserGroup usergroup_key(const WebHtml&h, std::string_view arg_name)
        {
            return usergroup_key(h.context(), arg_name);
        }
        
        UserGroup usergroup_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            return usergroup_key(arg_string);
        }
        
        UserGroup usergroup_key(const WebHtml&h, std::initializer_list<std::string_view> arg_names)
        {
            return usergroup_key(h.context(), arg_names);
        }
    }


    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    namespace cdb {
    
        namespace {
            std::vector<UserGroup>  all_usergroups_sorted()
            {
                static thread_local SQ    s("SELECT id FROM UserGroups ORDER BY k");
                return s.vec<UserGroup>();
            }
            
            std::vector<UserGroup>  all_usergroups_unsorted()
            {
                static thread_local SQ    s("SELECT id FROM UserGroups");
                return s.vec<UserGroup>();
            }
        }

        std::vector<UserGroup>      all_usergroups(Sorted sorted)
        {
            return sorted ? all_usergroups_sorted() : all_usergroups_unsorted();
        }
        
        size_t                  all_usergroups_count()
        {
            static thread_local SQ s("SELECT COUNT(1) FROM UserGroups");
            return s.size();
        }


        namespace {
            std::vector<User>     all_users_sorted()
            {
                static thread_local SQ    s("SELECT id FROM Users ORDER BY k");
                return s.vec<User>();
            }
            
            std::vector<User>     all_users_unsorted()
            {
                static thread_local SQ    s("SELECT id FROM Users");
                return s.vec<User>();
            }
        }

        std::vector<User>       all_users(Sorted sorted)
        {
            return sorted ? all_users_sorted() : all_users_unsorted();
        }
        
        size_t                  all_users_count()
        {
            static thread_local SQ s("SELECT COUNT(1) FROM Users");
            return s.size();
        }

        User                    db_user(Document doc, bool* wasCreated)
        {
            if(wasCreated)
                *wasCreated = false;
            if(folder(doc) != users_folder()){
                yError() << "Rejecting user " << key(doc) << " due to not being in the '.users' folder";        
                return User{};
            }
            std::string k   = base_key(doc);
            if(k.empty())
                return User();
            
            static thread_local SQ    i("INSERT OR FAIL INTO Users (k,id) VALUES (?,?)");
            auto i_lk   = i.af();
            
            i.bind(1, k);
            i.bind(2, doc.id);
            if(i.step(false) == SqlQuery::Done){
                if(wasCreated)
                    *wasCreated = true;
                return User{doc.id};
            } else if(exists_user(doc.id)){
                return User{doc.id};
            } else {
                yError() << "Unable to get the user from the database: " << k;
                return User{};
            }
        }
        
        User               db_user(Fragment f, bool* wasCreated)
        {
            return db_user(document(f),wasCreated);
        }

        User                db_user(std::string_view k, bool* wasCreated)
        {
            std::string     tfn = user_filename(k);
            return db_user(db_document(users_folder(), tfn), wasCreated);
        }
            
        UserGroup                db_usergroup(Document doc, bool* wasCreated)
        {
            if(wasCreated)
                *wasCreated = false;
            if(folder(doc) != users_folder()){
                yError() << "Rejecting usergroup " << key(doc) << " due to not being in the '.usergroups' folder";        
                return UserGroup{};
            }
            std::string k   = base_key(doc);
            if(k.empty())
                return UserGroup();
            
            static thread_local SQ    i("INSERT OR FAIL INTO UserGroups (k,id) VALUES (?,?)");
            auto i_lk   = i.af();
            
            i.bind(1, k);
            i.bind(2, doc.id);
            if(i.step(false) == SqlQuery::Done){
                if(wasCreated)
                    *wasCreated = true;
                return UserGroup{doc.id};
            } else if(exists_usergroup(doc.id)){
                return UserGroup{doc.id};
            } else {
                yError() << "Unable to get the usergroup from the database: " << k;
                return UserGroup{};
            }
        }
        
        UserGroup               db_usergroup(Fragment f, bool* wasCreated)
        {
            return db_usergroup(document(f),wasCreated);
        }

        UserGroup                db_usergroup(std::string_view k, bool* wasCreated)
        {
            std::string     tfn = usergroup_filename(k);
            return db_usergroup(db_document(users_folder(), tfn), wasCreated);
        }

        Document                document(User u)
        {
            return exists(u) ? Document{u.id} : Document{};
        }
    
        Document                document(UserGroup u)
        {
            return exists(u) ? Document{u.id} : Document{};
        }
    
        void                    erase(User u)
        {
            static thread_local SQ d("DELETE FROM Users WHERE id=?");
            d.exec(u.id);
        }
        
        void                    erase(UserGroup u)
        {
            static thread_local SQ d("DELETE FROM UserGroups WHERE id=?");
            d.exec(u.id);
        }

        bool                    exists(User u)
        {
            return exists_user(u.id);
        }

        bool                    exists(UserGroup u)
        {
            return exists_usergroup(u.id);
        }

        bool                    exists_user(uint64_t i)
        {
            static thread_local SQ s("SELECT 1 FROM Users WHERE id=? LIMIT 1");
            return s.present(i);
        }
        
        bool                    exists_usergroup(uint64_t i)
        {
            static thread_local SQ s("SELECT 1 FROM UserGroups WHERE id=? LIMIT 1");
            return s.present(i);
        }

        Image                   icon(User u)
        {
            static thread_local SQ    s("SELECT icon FROM Users WHERE id=? LIMIT 1");
            return s.as<Image>(u.id);
        }
        
        Image                   icon(UserGroup u)
        {
            static thread_local SQ    s("SELECT icon FROM UserGroups WHERE id=? LIMIT 1");
            return s.as<Image>(u.id);
        }

        User::Info              info(User u, bool autoKeyToName)
        {
            User::Info  ret;
            static thread_local SQ  s("SELECT k,name,icon FROM Users WHERE id=?");
            auto s_af = s.af();
            s.bind(1, u.id);
            if(s.step() == SqlQuery::Row){
                ret.key     = s.v_text(1);
                ret.name    = s.v_text(2);
                ret.doc     = Document{u.id};
                ret.icon    = Image(s.v_uint64(3));
                if(autoKeyToName && ret.name.empty())
                    ret.name    = ret.key;
                
            }
            return ret;
        }
        
        UserGroup::Info              info(UserGroup u, bool autoKeyToName)
        {
            UserGroup::Info  ret;
            static thread_local SQ  s("SELECT k,name,icon FROM UserGroups WHERE id=?");
            auto s_af = s.af();
            s.bind(1, u.id);
            if(s.step() == SqlQuery::Row){
                ret.key     = s.v_text(1);
                ret.name    = s.v_text(2);
                ret.doc     = Document{u.id};
                ret.icon    = Image(s.v_uint64(3));
                if(autoKeyToName && ret.name.empty())
                    ret.name    = ret.key;
                
            }
            return ret;
        }

        std::string             key(User u)
        {
            static thread_local SQ    s("SELECT k FROM Users WHERE id=?");
            return s.str(u.id);
        }
        
        std::string             key(UserGroup u)
        {
            static thread_local SQ    s("SELECT k FROM UserGroups WHERE id=?");
            return s.str(u.id);
        }

        User                    make_user(std::string_view k, const Root* rt)
        {
            if(!rt)
                rt  = wksp::root_first(DataRole::Users);
            if(!rt){
                yError() << "No root specified to create the user in!";
                return User{};
            }
            
            std::string     tfn = user_filename(k);
            Document    doc = db_document(users_folder(), tfn);
            bool            was = false;
            User         t   = db_user(doc, &was);
            if(!was)
                return t;
            if(fragments_count(doc))
                return t;
            User::SharedFile td  = write(t, rt);
            td -> name  = k;
            td -> save();
            return t;
        }
        
        UserGroup                    make_usergroup(std::string_view k, const Root* rt)
        {
            if(!rt)
                rt  = wksp::root_first(DataRole::Users);
            if(!rt){
                yError() << "No root specified to create the user in!";
                return UserGroup{};
            }
            
            std::string     tfn = usergroup_filename(k);
            Document    doc = db_document(users_folder(), tfn);
            bool            was = false;
            UserGroup         t   = db_usergroup(doc, &was);
            if(!was)
                return t;
            if(fragments_count(doc))
                return t;
            UserGroup::SharedFile td  = write(t, rt);
            td -> name  = k;
            td -> save();
            return t;
        }

        User::SharedData         merged(User u, unsigned int opts)
        {
            User::SharedData  ret = std::make_shared<User::Data>();
            for(auto& i : reads(u)){
                if(opts & IsUpdate)
                    cdb::update(i.first);
                ret -> merge(*(i.second), static_cast<bool>(opts&Override));
            }
            return ret;
        }
        
        UserGroup::SharedData          merged(UserGroup u, unsigned int opts)
        {
            UserGroup::SharedData  ret = std::make_shared<UserGroup::Data>();
            for(auto& i : reads(u)){
                if(opts & IsUpdate)
                    cdb::update(i.first);
                ret -> merge(*(i.second), static_cast<bool>(opts&Override));
            }
            return ret;
        }

        std::vector<UserFragDoc> reads(User u)
        {
            std::vector<UserFragDoc>  ret;
            for(Fragment f : fragments(document(u), DataRole::Users)){
                User::SharedFile  p   = user_doc(f);
                if(p)
                    ret.push_back(UserFragDoc(f,p));
            }
            return ret;
        }
        
        std::vector<UserFragDoc> reads(User u, class Root*rt)
        {
            std::vector<UserFragDoc>  ret;
            for(Fragment f : fragments(document(u), rt)){
                User::SharedFile  p   = user_doc(f);
                if(p)
                    ret.push_back(UserFragDoc(f,p));
            }
            return ret;
        }

        std::vector<UserGroupFragDoc> reads(UserGroup u)
        {
            std::vector<UserGroupFragDoc>  ret;
            for(Fragment f : fragments(document(u), DataRole::Users)){
                UserGroup::SharedFile  p   = usergroup_doc(f);
                if(p)
                    ret.push_back(UserGroupFragDoc(f,p));
            }
            return ret;
        }
        
        std::vector<UserGroupFragDoc> reads(UserGroup u, class Root*rt)
        {
            std::vector<UserGroupFragDoc>  ret;
            for(Fragment f : fragments(document(u), rt)){
                UserGroup::SharedFile  p   = usergroup_doc(f);
                if(p)
                    ret.push_back(UserGroupFragDoc(f,p));
            }
            return ret;
        }

        User                    user(std::string_view k)
        {
            static thread_local SQ    s("SELECT id FROM Users WHERE k=?");
            return s.as<User>(k);
        }
        
        User                    user(uint64_t i)
        {
            return exists_user(i) ? User{i} : User{};
        }
        
        User::SharedFile        user_doc(Fragment f, bool fAllowEmpty)
        {
            if(!f)
                return User::SharedFile();

            auto ch   = frag_bytes(f);
            if(ch.empty())
                return fAllowEmpty ? std::make_shared<User::File>() : User::SharedFile();
            
            User::SharedFile  td = std::make_shared<User::File>();
            std::filesystem::path   fp  = path(f);
            if(!td->load(std::move(ch), fp)){
                yError() << "Unable to read " << fp;
                return User::SharedFile();
            }
            td -> set_file(fp);
            return td;
        }
        
        UserGroup                    usergroup(std::string_view k)
        {
            static thread_local SQ    s("SELECT id FROM UserGroups WHERE k=?");
            return s.as<UserGroup>(k);
        }
        
        UserGroup                    usergroup(uint64_t i)
        {
            return exists_user(i) ? UserGroup{i} : UserGroup{};
        }
        
        UserGroup::SharedFile        usergroup_doc(Fragment f, bool fAllowEmpty)
        {
            if(!f)
                return UserGroup::SharedFile();

            auto ch   = frag_bytes(f);
            if(ch.empty())
                return fAllowEmpty ? std::make_shared<UserGroup::File>() : UserGroup::SharedFile();
            
            UserGroup::SharedFile  td = std::make_shared<UserGroup::File>();
            std::filesystem::path   fp  = path(f);
            if(!td->load(std::move(ch), fp)){
                yError() << "Unable to read " << fp;
                return UserGroup::SharedFile();
            }
            td -> set_file(fp);
            return td;
        }
        

        User::SharedFile          write(User u, const Root*rt)
        {
            Document    d   = document(u);
            if(!d)
                return User::SharedFile();
            if(rt && !rt->is_writable(DataRole::Users))
                return User::SharedFile();
            Fragment    f   = rt ? fragment(d, rt) : writable(d, DataRole::Users);
            if(f)
                return user_doc(f, true);

            Folder      fo  = folder(d);
            if((fo != cdb::users_folder()) && !exists(fo, rt))
                make_directory(fo, rt);

            User::SharedFile ptr  = std::make_shared<User::File>();
            ptr -> set_file( rt -> resolve(key(d)));
            ptr -> reload();
            return ptr;
        }    

        UserGroup::SharedFile          write(UserGroup u, const Root*rt)
        {
            Document    d   = document(u);
            if(!d)
                return UserGroup::SharedFile();
            if(rt && !rt->is_writable(DataRole::Users))
                return UserGroup::SharedFile();
            Fragment    f   = rt ? fragment(d, rt) : writable(d, DataRole::Users);
            if(f)
                return usergroup_doc(f, true);

            Folder      fo  = folder(d);
            if((fo != cdb::users_folder()) && !exists(fo, rt))
                make_directory(fo, rt);

            UserGroup::SharedFile ptr  = std::make_shared<UserGroup::File>();
            ptr -> set_file( rt -> resolve(key(d)));
            ptr -> reload();
            return ptr;
        }    
    }
}
