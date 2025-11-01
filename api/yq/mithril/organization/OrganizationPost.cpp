////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "OrganizationPost.hpp"

#include <yq/mithril/organization/OrganizationArg.hpp>
#include <yq/mithril/web/WebContext.hpp>
#include <yq/mithril/organization/Organization.hpp>
#include <yq/text/copy.hpp>

namespace yq::mithril::post {
    Organization organization(WebContext&ctx, bool *detected)
    {
        ctx.decode_post();
        
        if(detected)
            *detected   = false;
            
        std::string    k    = ctx.rx_post.first("organization");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return arg::organization_id(k);
        }
        return Organization();
    }
    
    Organization organization(WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_name));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::organization_id(arg_string);
    }
    
    Organization organization(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_names));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::organization_id(arg_string);
    }
}
