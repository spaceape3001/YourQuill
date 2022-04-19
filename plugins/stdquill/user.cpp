////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "stdquill.hpp"

#include <db/cdb_sq.hpp>
#include <db/user_arg.hpp>
#include <db/user_cdb.hpp>
#include <db/user_data.hpp>
#include <db/user_html.hpp>


#include <yq/srv/NotifyAdapters.hpp>
#include <yq/srv/Stage3.hpp>

using namespace yq;
using namespace yq::arg;
using namespace yq::cdb;
using namespace yq::html;

namespace {



    
    
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
        User::SharedData data = cdb::merged(u, cdb::IS_UPDATE);
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
        User::SharedData data = cdb::merged(u, cdb::IS_UPDATE);
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
        
        User::SharedData data = cdb::merged(t, cdb::IS_UPDATE);
        update_info(t, data);
    }
    
    void    update_user_image(Fragment frag, Change change)
    {
    }
    
    
    //  -------------------------------------
    //  pages
    
    

    
    
    
    //  -------------------------------------
    void    reg_me()
    {
        
        
        
        

        on_stage3<stage3_user1>(s3_user1, cdb::users_folder(), "*.user");
        on_stage3<stage3_user2>(s3_user2, cdb::users_folder(), "*.user");
     
        on_change<update_user_data>(cdb::users_folder(), "*.user");
     
        
        
        
    }
}


YQ_INVOKE(reg_me();)
