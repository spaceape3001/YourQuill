////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Authentication.hpp"
#include "QuillData.hpp"
#include "QuillFile.hpp"

#include "AuthAccept.hpp"
#include "AuthReject.hpp"
#include "AuthSimple.hpp"

#include "User.hpp"
#include "UserArg.hpp"
#include "UserCDB.hpp"
#include "UserData.hpp"
#include "UserFile.hpp"
#include "UserHtml.hpp"
#include "UserPost.hpp"

#include <yq/bit/KeyValue.hpp>
#include <basic/c_utils.hpp>
#include <yq/db/HtmlLayout.hpp>
#include <yq/db/SQ.hpp>
#include <yq/file/DocumentCDB.hpp>
#include <yq/file/DocumentHtml.hpp>
#include <yq/file/FolderCDB.hpp>
#include <yq/file/FragmentCDB.hpp>
#include <yq/file/Root.hpp>
#include <yq/file/RootHtml.hpp>
#include <yq/image/ImageCDB.hpp>
#include <yq/io/Strings.hpp>
#include <yq/meta/ObjectInfoWriter.hpp>
#include <yq/stream/Ops.hpp>
#include <yq/text/text_utils.hpp>
#include <yq/web/WebContext.hpp>
#include <yq/web/WebHtml.hpp>
#include <yq/wksp/Workspace.hpp>

#include <yq/db/IDLock.ipp>

YQ_OBJECT_IMPLEMENT(yq::Authentication)
YQ_OBJECT_IMPLEMENT(yq::auth::AutoAccept)
YQ_OBJECT_IMPLEMENT(yq::auth::AutoReject)
YQ_OBJECT_IMPLEMENT(yq::auth::Simple)


namespace yq {

    template class IDLock<User>;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    YQ_INVOKE( 
        auto w = writer<auth::Simple>();
        w.label("Simple");
        w.description("For Testing Purposes, UNECRYPTED password authentication");
        w.property("password", &auth::Simple::m_password);

        writer<auth::AutoAccept>().label("Accept").description("For Testing Purposes, auto-accepting authentication");
        writer<auth::AutoReject>().label("Reject").description("For Testing Purposes, auto-rejecting authentication");
    )



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    void    QuillData::reset()
    {
        *this = {};
    }

////////////////////////////////////////////////////////////////////////////////



    void    QuillFile::reset() 
    {
        QuillData::reset();
    }

    namespace {
        QuillData::Root    parse_root(const KeyValue& a)
        {
            QuillData::Root ret;
            ret.path    = trimmed(a.data);
            ret.key     = trimmed(a.value(kv::key( "key", "k" )));
            ret.color   = a.value(kv::key("color"));
            ret.vcs     = Vcs(a.value(kv::key("vcs")));
            ret.name    = a.value(kv::key("name"));
            ret.icon    = a.value(kv::key("icon"));
            for(DataRole dr : DataRole::all_values())
                ret.policy[dr]  = Access(a.value(dr.key()));
            return ret;
        }
    }

    bool    QuillFile::read(KVTree&&attrs, std::string_view) 
    {
        abbr            = attrs.value(kv::key( "abbr", "abbreviation" ));
        author          = attrs.value(kv::key("author"));
        aux_ports       = attrs.values_set_u16("aux");
        cache           = attrs.value(kv::key("cache"));
        
        const KeyValue   *a  = nullptr;;
        if((a = attrs.first(kv::key("disclaimer", "disclaim", "deny")))){
            copyright.stance  = AssertDeny::Deny;
        } else if((a = attrs.first(kv::key("notice")))){
            copyright.stance  = AssertDeny::Neither;
        } else if((a = attrs.first(kv::key("copyright")))){
            copyright.stance  = AssertDeny::Assert;
        }
        if(a){
            copyright.text    = a->data;
            copyright.from    = to_ushort(a->value(kv::key("from"))).value;
            copyright.to      = to_ushort(a->value(kv::key("to"))).value;
        }
        
        home            = attrs.value(kv::key("home"));
        ini             = attrs.value(kv::key("ini"));
        local_user      = attrs.value(kv::key("local", "user", "local_user"));
        log_dir         = attrs.value(kv::key("logs"));
        name            = attrs.value(kv::key("name", "%"));
        port            = to_uint16(attrs.value(kv::key("port"))).value;
        read_timeout    = to_uinteger(attrs.value(kv::key("timeout"))).value;
        threads         = to_uinteger(attrs.value(kv::key("threads"))).value;
        temp_dir        = attrs.value(kv::key("temp", "tmp", "tempdir", "temp_dir"));
        
        attrs.all(kv::key("root", "r"), [&](const KeyValue& a){
            roots << parse_root(a);
        });
        attrs.all(kv::key("template", "t"), [&](const KeyValue& a){
            templates << parse_root(a);
        });
        return true;
    }

    namespace {
        void    write_onto(KeyValue& a, const QuillData::Root& r)
        {
            a.data = r.path;
            if(!r.key.empty())
                a << KeyValue(szKey, r.key);
            if(!r.name.empty())
                a << KeyValue(szName, r.name);
            if(!r.color.empty())
                a << KeyValue(szColor, r.color);
            if(!r.icon.empty())
                a << KeyValue(szIcon, r.icon);
            if(r.vcs != Vcs())
                a << KeyValue(szVcs, r.vcs.key());
            for(DataRole dr : DataRole::all_values()){
                if(r.policy[dr] != Access())
                    a << KeyValue(dr.key(), r.policy[dr].key());
            }
        }
    }

    bool    QuillFile::write(KVTree& attrs) const
    {
        if(!name.empty())
            attrs << KeyValue(szName, name);
        if(!abbr.empty())
            attrs << KeyValue("abbr", abbr);
        if(!author.empty())
            attrs << KeyValue(szAuthor, author);
        if(!aux_ports.empty())
            attrs << KeyValue("aux", join(aux_ports, ", "));
        if(!cache.empty())
            attrs << KeyValue(szCache, cache);
        if(!copyright.empty()){
            KeyValue  a;
            switch(copyright.stance){
            case AssertDeny::Neither:
                a.key   = szNotice;
                break;
            case AssertDeny::Deny:
                a.key   = "disclaim";
                break;
            case AssertDeny::Assert:
                a.key   = "copyright";
                break;
            default:
                break;
            }
            if(!copyright.text.empty())
                a.data = copyright.text;
            if(copyright.from)
                a << KeyValue("from", to_string(copyright.from));
            if(copyright.to)
                a << KeyValue("to", to_string(copyright.to));
            attrs << a;
        }
        if(!home.empty())
            attrs << KeyValue("home", home);
        if(!ini.empty())
            attrs << KeyValue("ini", ini);
        if(!local_user.empty())
            attrs << KeyValue("local", local_user);
        if(!log_dir.empty())
            attrs << KeyValue("logs", log_dir);
        if(port)
            attrs << KeyValue("port", to_string(port));
        if(read_timeout)
            attrs << KeyValue("timeout", to_string(read_timeout));
        if(!temp_dir.empty())
            attrs << KeyValue("temp", temp_dir);
        if(threads)
            attrs << KeyValue("threads", to_string(threads));
        
        for(const auto & r : roots){
            KeyValue   a;
            a.key   = "root";
            write_onto(a, r);
            attrs << a;
        }
        for(const auto& r : templates){
            KeyValue   a;
            a.key   = "template";
            write_onto(a, r);
            attrs << a;
        }
        return true;
    }
    
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    User::Data&  User::Data::merge(const Data&b, bool fOverride)
    {
        set_if_empty(name, b.name, fOverride);
        bio += b.bio;
        permissions |= b.permissions;
        set_if_empty(brief, b.brief, fOverride);
        if(fOverride || !authentication)
            authentication  = b.authentication;
        return *this;
    }
    
    void  User::Data::reset()
    {
        *this   = Data();
    }

////////////////////////////////////////////////////////////////////////////////

    void  User::File::reset() 
    {
        Data::reset();
    }

    bool        User::File::read(KVTree&&attrs, std::string_view body, std::string_view fname) 
    { 
        name        = attrs.value(szName);
        brief       = attrs.value(szBrief);
        permissions = Permissions(attrs.value(szPermission));
        
        const KeyValue* a   = attrs.first(szAuthentication);
        if(a){
            authentication = Authentication::load(*a);
            if(!authentication){
                yError() << "Bad Authentication on file " << fname;
                return false;
            }
        }
        
        if(!skip_bio){
            if(!read_kv(bio, body))
                return false;
        }
        
        return true; 
    }
    
    bool        User::File::write(KVTree&attrs, Stream&str) const
    {
        if(!name.empty())
            attrs.set(szName, name);
        if(!brief.empty())
            attrs.set(szBrief, brief);
        if(permissions != Permissions())
            attrs.set(szPermission, permissions.as_string());
        if(authentication){
            KeyValue        kva;
            kva.key         = szAuthentication;
            authentication -> save(kva);
            attrs << kva;
        }
        write_kv(str, bio);
        return true;
    }
    
    
    //bool  User::File::read(const XmlDocument&doc, std::string_view fname) 
    //{
        //const XmlNode*  xn  = doc.first_node(szYQUser);
        //if(!xn){
            //yError() << "No appropriate root element!  In: '" << fname << "'";
            //return false;
        //}
        
        //xn      = xn -> first_node(szUser);
        //if(!xn){
            //yError() << "No user! In: " << fname << "'";
            //return false;
        //}
        
        //name            = read_child(xn, szName, x_string);
        //brief           = read_child(xn, szBrief, x_string);
        //authentication  = read_child(xn, szAuthentication, x_authentication);
        //if(!skip_bio)
            //bio         = read_children(xn, szContext, x_context);
        //std::vector<std::string>    theGroups = read_children(xn, szGroup, x_string);
        //permissions     = read_child(xn, szPermission, x_flag<Permission>);
        //return true;
    //}
    
    //bool  User::File::write(XmlDocument&doc) const 
    //{
        //XmlNode*  xroot  = doc.allocate_node(rapidxml::node_element, szYQUser);
        //doc.append_node(xroot);
        //XmlNode*    xuser   = doc.allocate_node(rapidxml::node_element, szUser);
        //xroot -> append_node(xuser);
        
        //write_child(xuser, szName, name);
        //if(!brief.empty())
            //write_child(xuser, szBrief, brief);
        //write_child(xuser, szAuthentication, authentication);
        //write_children(xuser, szContext, bio);
        //if(permissions)
            //write_child(xuser, szPermission, permissions);
        //return true;
    //}

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

    namespace cdb {
        namespace {
            inline std::string user_filename(std::string_view k)
            {
                return make_filename(k, User::szExtension);
            }

            Vector<User>     all_users_sorted()
            {
                static thread_local SQ    s("SELECT id FROM Users ORDER BY k");
                return s.vec<User>();
            }
            
            Vector<User>     all_users_unsorted()
            {
                static thread_local SQ    s("SELECT id FROM Users");
                return s.vec<User>();
            }
        }

        Vector<User>       all_users(Sorted sorted)
        {
            return sorted ? all_users_sorted() : all_users_unsorted();
        }
        
        size_t                  all_users_count()
        {
            static thread_local SQ s("SELECT COUNT(1) FROM Users");
            return s.size();
        }

        bool                    any_users()
        {
            static thread_local SQ s("SELECT 1 FROM Users LIMIT 1");
            auto af = s.af();
            return s.step() == SqlQuery::Row;
        }

        std::string             brief(User u)
        {
            static thread_local SQ s("SELECT brief FROM Users WHERE id=?");
            return s.str(u.id);
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
            

        Document                document(User u)
        {
            return exists(u) ? Document{u.id} : Document{};
        }
    
        void                    erase(User u)
        {
            static thread_local SQ d("DELETE FROM Users WHERE id=?");
            d.exec(u.id);
        }
        
        bool                    exists(User u)
        {
            return exists_user(u.id);
        }

        bool                    exists_user(uint64_t i)
        {
            static thread_local SQ s("SELECT 1 FROM Users WHERE id=? LIMIT 1");
            return s.present(i);
        }
        
        Image                   icon(User u)
        {
            static thread_local SQ    s("SELECT icon FROM Users WHERE id=? LIMIT 1");
            return s.as<Image>(u.id);
        }
        
        User::Info              info(User u, bool autoKeyToName)
        {
            User::Info  ret;
            static thread_local SQ  s("SELECT k,name,icon,brief,is_admin,is_owner,is_reader,is_writer FROM Users WHERE id=?");
            auto s_af = s.af();
            s.bind(1, u.id);
            if(s.step() == SqlQuery::Row){
                ret.key     = s.v_text(1);
                ret.name    = s.v_text(2);
                ret.doc     = Document{u.id};
                ret.icon    = Image(s.v_uint64(3));
                ret.brief   = s.v_text(4);
                if(autoKeyToName && ret.name.empty())
                    ret.name    = ret.key;
                ret.is_admin    = s.v_bool(5);
                ret.is_owner    = s.v_bool(6);
                ret.is_reader   = s.v_bool(7);
                ret.is_writer   = s.v_bool(8);
            }
            return ret;
        }
        

        bool                    is_admin(User u)
        {
            static thread_local SQ s("SELECT is_admin FROM Users WHERE id=?");
            return s.boolean(u.id);
        }
        
        bool                    is_owner(User u)
        {
            static thread_local SQ s("SELECT is_owner FROM Users WHERE id=?");
            return s.boolean(u.id);
        }
        
        bool                    is_reader(User u)
        {
            static thread_local SQ s("SELECT is_reader FROM Users WHERE id=?");
            return s.boolean(u.id);
        }
        
        bool                    is_writer(User u)
        {
            static thread_local SQ s("SELECT is_writer FROM Users WHERE id=?");
            return s.boolean(u.id);
        }
        

        std::string             key(User u)
        {
            static thread_local SQ    s("SELECT k FROM Users WHERE id=?");
            return s.str(u.id);
        }
        
        std::string             label(User u)
        {
            static thread_local SQ    s("SELECT ifnull(name,k) FROM Users WHERE id=?");
            return s.str(u.id);
        }

        User                    make_user(std::string_view k, const Root* rt, cdb_options_t opts, bool* wasCreated)
        {
            if(wasCreated)
                *wasCreated = false;
            if(k.empty()){
                yError() << "Cannot create a BLANK user.";
                return User();
            }
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
            if(wasCreated)
                *wasCreated = was;
            if(!was)
                return t;
            if(fragments_count(doc))
                return t;
            User::SharedFile td  = write(t, rt, opts);
            td -> name  = k;
            td -> save();
            return t;
        }
        

        User::SharedData         merged(User u, cdb_options_t opts)
        {
            if(!u)
                return User::SharedData();
            
            User::Lock  lk;
            if(!(opts & DONT_LOCK)){
                lk      = User::Lock::read(u);
                if(!lk){
                    yWarning() << "Unable to get read lock on user: " << key(u);
                    return User::SharedData();
                }
            }
            
            User::SharedData  ret = std::make_shared<User::Data>();
            for(auto& i : reads(u, opts)){
                if(opts & IS_UPDATE)
                    cdb::update(i.first);
                ret -> merge(*(i.second), static_cast<bool>(opts&OVERRIDE));
            }
            return ret;
        }
        
        std::string             name(User u)
        {
            static thread_local SQ    s("SELECT name FROM Users WHERE id=?");
            return s.str(u.id);
        }

        Vector<UserFragDoc> reads(User u, cdb_options_t opts)
        {
            Vector<UserFragDoc>  ret;
            for(Fragment f : fragments(document(u), DataRole::Users)){
                User::SharedFile  p   = user_doc(f, opts);
                if(p)
                    ret.push_back(UserFragDoc(f,p));
            }
            return ret;
        }
        
        Vector<UserFragDoc> reads(User u, class Root*rt, cdb_options_t opts)
        {
            Vector<UserFragDoc>  ret;
            for(Fragment f : fragments(document(u), rt)){
                User::SharedFile  p   = user_doc(f, opts);
                if(p)
                    ret.push_back(UserFragDoc(f,p));
            }
            return ret;
        }

        void                update_icon(User x)
        {
            if(!x)
                return ;
                
            Document    doc     = document(x);
            Image       img     = best_image(doc);
            static thread_local SQ u1("UPDATE Users SET icon=? WHERE id=?");
            static thread_local SQ u2("UPDATE Documents SET icon=? WHERE id=?");
            
            if(icon(x) != img){
                u1.exec(img.id, x.id);
                u2.exec(doc.id, x.id);
            }
        }

        User::SharedData         update(User x, cdb_options_t opts)
        {
            if(!x)
                return User::SharedData();

            if(opts & U_ICON)
                update_icon(x);
        
            auto data  = merged(x, opts|IS_UPDATE);
            if(!data)
                return User::SharedData();

            static thread_local SQ uInfo("UPDATE Users SET name=?,brief=?,is_owner=?,is_admin=?,is_writer=?,is_reader=? WHERE id=?");
            if(opts & U_INFO){
                uInfo.bind(1, data->name);
                uInfo.bind(2, data->brief);
                uInfo.bind(3, data->permissions.is_set( Permission::Owner ));
                uInfo.bind(4, data->permissions.is_set( Permission::Admin ));
                uInfo.bind(5, data->permissions.is_set( Permission::Writer ));
                uInfo.bind(6, data->permissions.is_set( Permission::Reader ));
                uInfo.bind(7, x.id);
                uInfo.exec();
            }

            return data;
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

        User                    user(Document doc, bool calc)
        {
            if(!doc)
                return User();
            if(exists_user(doc.id))
                return User{doc.id};
            if(calc && (folder(doc) == users_folder())){
                std::string k   = skeyb(doc);
                return user(k);
            }
            return User();
        }
        
        User::SharedFile        user_doc(Fragment f, cdb_options_t opts)
        {
            if(!f)
                return User::SharedFile();
                
            std::filesystem::path   fp  = path(f);

            Fragment::Lock  lk;
            if(!(opts & DONT_LOCK)){
                lk      = Fragment::Lock::read(f);
                if(!lk){
                    yWarning() << "Unable to obtain read lock on fragment: " << fp;
                    return User::SharedFile();
                }
            }

            auto ch   = frag_bytes(f, opts);
            lk.free();
            if(ch.empty()){
                if(opts & ALLOW_EMPTY)
                    return std::make_shared<User::File>();
                return User::SharedFile();
            }
            
            User::SharedFile  td = std::make_shared<User::File>();
            if(!td->load(std::move(ch), fp)){
                yError() << "Unable to parse user file: " << fp;
                return User::SharedFile();
            }
            td -> set_file(fp);
            return td;
        }
        

        User::SharedFile          write(User u, const Root*rt, cdb_options_t opts)
        {
            if(!u)
                return User::SharedFile();
                
            Document    d   = document(u);
            if(!d){
                yWarning() << "write(User '" << key(u) << "'): Has no document!";
                return User::SharedFile();
            }
            if(rt && !rt->is_writable(DataRole::Users)){
                yWarning() << "write(User '" << key(u) << "'): Root " << rt->key << " cannot be written to!";
                return User::SharedFile();
            }
            Fragment    f   = rt ? fragment(d, rt) : writable(d, DataRole::Users);
            if(f)
                return user_doc(f, opts);

            Folder      fo  = folder(d);
            if((fo != cdb::users_folder()) && !exists(fo, rt))
                make_directory(fo, rt);

            User::SharedFile ptr  = std::make_shared<User::File>();
            ptr -> set_file( rt -> resolve(key(d)));
            ptr -> reload();
            return ptr;
        }    
    }
    
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

