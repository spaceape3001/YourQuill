////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/atom/AtomPost.hpp>
#include <yq/mithril/atom/AtomArg.hpp>
#include <yq/mithril/web/WebContext.hpp>
#include <yq/mithril/atom/Atom.hpp>
#include <yq/text/copy.hpp>

namespace yq::mithril::post {
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
