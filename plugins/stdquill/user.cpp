////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "stdquill.hpp"
#include <yq/srv/NotifyAdapters.hpp>
#include <yq/srv/Stage3.hpp>
#include <yq/user/arg.hpp>
#include <yq/user/cdb.hpp>
#include <yq/user/UserData.hpp>
#include <yq/wksp/CacheFwd.hpp>
#include <yq/wksp/CacheSQ.hpp>



WebHtml&    operator<<(WebHtml&h, const Dev<User>&v)
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

WebHtml&    operator<<(WebHtml&h, const DevID<User>&v)
{
    if(v.data)
        h << "<a href=\"/dev/user?id=" << v.data.id << "\">";
    h << v.data.id;
    if(v.data)
        h << "</a>";
    return h;
}

WebHtml&    operator<<(WebHtml&h, const User&u)
{
    h << "(user: " << cdb::name(u) << ")";
    return h;
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


namespace {
    void    var_can_edit(Stream& str, WebContext&)
    {
        //  TODO
        str << true;
    }

    void    var_local(Stream& str, WebContext& ctx)
    {
        str << ctx.is_local();
    }

    void    var_logged_in(Stream&str, WebContext& ctx)
    {
        //  TODO
        str << true;
    }

    void    var_ssid(Stream&str, WebContext& ctx)
    {
        str << ctx.session.ssid;
    }
    
    
    //  -------------------------------------
    //  update
    
    void    update_info(User x, const User::SharedData& data)
    {
        static thread_local cdb::SQ u("UPDATE Users SET name=?,brief=? WHERE id=?");
        auto u_af = u.af();
        u.bind(1, data->name);
        u.bind(2, data->brief);
        u.bind(3, x.id);
        u.exec();
    }
    
    
    void    stage3_user1(Document doc)
    {
        User u  = cdb::db_user(doc);
        User::SharedData data = cdb::merged(u, cdb::IsUpdate);
        if(data)
            update_info(u, data);
    }

    //void    stage3_images(Document doc)
    //{
        //UserGroup u  = cdb::db_usergroup(doc);
        //UserGroup::SharedData data = cdb::merged(u, cdb::IsUpdate);
        //if(data)
            //update_info(u, data);
    //}

    void    stage3_user2(Document doc)
    {
        User u  = cdb::db_user(doc);
        User::SharedData data = cdb::merged(u, cdb::IsUpdate);
        if(!data)
            return ;
        
        //  check for images....
        
    }
    
    
    void    update_user_data(Fragment frag, Change change)
    {
        Document    doc = cdb::document(frag);
        User        t   = cdb::db_user(doc);

        if(change == Change::Removed){
            if(cdb::fragments_count(doc) <= 1){
                cdb::erase(t);
                return ;
            }
        }
        
        User::SharedData data = cdb::merged(t, cdb::IsUpdate);
        update_info(t, data);
    }
    
    void    update_user_image(Fragment frag, Change change)
    {
    }
    
    
    //  -------------------------------------
    //  pages
    
    void    page_user(WebHtml& h)
    {
        User    u   = arg::user(h);
        if(!u)
            throw hNotFound;
        
        h.title() << "User: " << cdb::name(u);
        h.p() << "Fill in the blank... TODO!";
    }
    
    void    page_user_list(WebHtml&h)
    {
        // might have some characteristics?
        h.title() << "Users";
        dev_table(h, cdb::all_users(Sorted::YES));  // STUB
    }

    void    dev_page_users(WebHtml& h)
    {
        h.title() << "All Users";
        dev_table(h, cdb::all_users(Sorted::YES));
    }
    
    //  -------------------------------------
    void    reg_me()
    {
        reg_webvar<var_can_edit>("can_edit");
        reg_webvar<var_logged_in>("logged_in");
        reg_webvar<var_local>("local");
        reg_webvar<var_ssid>("ssid");

        on_stage3<stage3_user1>(s3_user1, cdb::users_folder(), "*.user");
        on_stage3<stage3_user2>(s3_user2, cdb::users_folder(), "*.user");
     
        on_change<update_user_data>(cdb::users_folder(), "*.user");
     
        reg_webpage<page_user>("/user");
        reg_webpage<page_user_list>("/admin/users");
        reg_webpage<dev_page_users>("/dev/users");
    }
}


YQ_INVOKE(reg_me();)
