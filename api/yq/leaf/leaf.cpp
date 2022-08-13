////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <kernel/leaf/LeafCDB.hpp>
#include <kernel/leaf/LeafFile.hpp>
#include <kernel/leaf/LeafLog.hpp>
#include "LeafArg.hpp"
#include "LeafHtml.hpp"
#include "LeafPost.hpp"

#include <basic/CollectionUtils.hpp>
#include <kernel/db/NKI.hpp>
#include <kernel/db/SQ.hpp>
#include <kernel/file/DocumentCDB.hpp>
#include <kernel/file/FolderCDB.hpp>
#include <kernel/file/FragmentCDB.hpp>
#include <kernel/file/Root.hpp>
#include <kernel/image/ImageCDB.hpp>
#include <basic/FileUtils.hpp>
#include <kernel/org/TagCDB.hpp>
#include <basic/StreamOps.hpp>
#include <basic/stream/Text.hpp>
#include <yq/web/WebContext.hpp>
#include <yq/web/WebHtml.hpp>

namespace yq {

////////////////////////////////////////////////////////////////////////////////
//  LEAF
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

    namespace arg {
        Leaf leaf(std::string_view  arg_string)
        {
            arg_string   = trimmed(arg_string);
            if(arg_string.empty())
                return Leaf{};
                
            Leaf t   = cdb::leaf( arg_string);
            if(t)
                return t;
            t       = cdb::leaf_by_title(arg_string);
            if(t)
                return t;
                
            uint64_t    i = to_uint64( arg_string).value;
            if(cdb::exists_leaf(i))
                return Leaf{i};
            return Leaf{};
        }
        
        Leaf leaf(const WebContext&ctx, bool *detected)
        {
            std::string    k    = ctx.find_query("id");
            if(!k.empty())
                return leaf_id(k);
            
            k       = ctx.find_query("key");
            if(!k.empty())
                return leaf_key(k);
            
            k       = ctx.find_query("leaf");
            if(!k.empty())
                return leaf(k);
            return Leaf{};
        }
        
        
        Leaf leaf(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return leaf(arg_string);
        }
        
        Leaf leaf(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return leaf(arg_string);
        }

        Leaf leaf_id(std::string_view arg_string)
        {
            uint64_t    i   = to_uint64(arg_string).value;
            if(cdb::exists_leaf(i))
                return Leaf{i};
            return Leaf{};
        }

        Leaf leaf_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return leaf_id(arg_string);
        }
        
        Leaf leaf_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return leaf_id(arg_string);
        }
        
        Leaf leaf_key(std::string_view arg_string)
        {
            return cdb::leaf(trimmed(arg_string));
        }
        
        Leaf leaf_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return leaf_key(arg_string);
        }
        
        Leaf leaf_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return leaf_key(arg_string);
        }

        Leaf leaf_title(std::string_view arg_string)
        {
            return cdb::leaf_by_title(trimmed(arg_string));
        }
        
        Leaf leaf_title(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return leaf_title(arg_string);
        }
        
        Leaf leaf_title(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return leaf_title(arg_string);
        }
    }

////////////////////////////////////////////////////////////////////////////////
    


////////////////////////////////////////////////////////////////////////////////

    namespace html {
        WebHtml&    operator<<(WebHtml& h, Leaf l)
        {
            Thumbnail th = cdb::thumbnail(cdb::icon(l), h.context().session.icon_size);
            
            // start the url (later)
            if(th)
                h << th << " ";
            h << cdb::label(l);
            //  end the url(later)
            return h;
        }

        WebHtml&    operator<<(WebHtml&h, Dev<Leaf> v)
        {
            h << "<a href=\"/dev/leaf?id=" << v.data.id << "\">";
            h << "[" << v.data.id << "] " << cdb::label(v.data);
            h << "</a>";
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, DevID<Leaf> v)
        {
            if(v.data)
                h << "<a href=\"/dev/leaf?id=" << v.data.id << "\">";
            h << v.data.id;
            if(v.data)
                h << "</a>";
            return h;
        }
    
        void        dev_table(WebHtml&h, const std::vector<Leaf>&leafs)
        {
            auto t=h.table();
            h << "<tr><th><ID></th><th>key</th><th>Title</th></tr>\n";
            for(Leaf l : leafs){
                auto    i   = cdb::info(l);
                h << "<tr><td>" << dev_id(l) << "</td><td><a href=\"/leaf?id=" << l.id << "\">"
                    << i.key << "<a></td><td>" << i.title << "</td></tr>\n";
            }
        }
    }

////////////////////////////////////////////////////////////////////////////////

    namespace post {

        Leaf leaf(WebContext&ctx, bool *detected)
        {
            ctx.decode_post();
            
            if(detected)
                *detected   = false;
                
            std::string    k    = ctx.rx_post.first("leaf");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return arg::leaf_id(k);
            }
            return Leaf();
        }
        
        Leaf leaf(WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_name));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::leaf_id(arg_string);
        }
        
        Leaf leaf(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_names));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::leaf_id(arg_string);
        }
    }

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}

