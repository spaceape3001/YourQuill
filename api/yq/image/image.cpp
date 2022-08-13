////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Graph.hpp"
#include "GraphCDB.hpp"
#include "Image.hpp"
#include "ImageArg.hpp"
#include "ImageCDB.hpp"
#include "ImageHtml.hpp"
#include "ImagePost.hpp"

#include <yq/file/DocumentCDB.hpp>
#include <yq/file/FolderCDB.hpp>
#include <yq/file/FragmentCDB.hpp>
#include <yq/file/FragmentHtml.hpp>
#include <kernel/file/Root.hpp>
#include <basic/StreamOps.hpp>
#include <basic/TextUtils.hpp>
#include <basic/ByteArray.hpp>
#include <yq/web/WebContext.hpp>
#include <yq/web/WebHtml.hpp>

namespace yq {
////////////////////////////////////////////////////////////////////////////////
//  GRAPH
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  IMAGE
////////////////////////////////////////////////////////////////////////////////

    namespace arg {
        Image image(std::string_view  arg_string)
        {
            arg_string   = trimmed(arg_string);
            if(arg_string.empty())
                return Image{};
            uint64_t    i = to_uint64( arg_string).value;
            if(cdb::exists_image(i))
                return Image{i};
            return Image{};
        }
        
        Image image(const WebContext&ctx, bool *detected)
        {
            std::string    k    = ctx.find_query("id");
            if(!k.empty())
                return image(k);
            k       = ctx.find_query("image");
            if(!k.empty())
                return image(k);
            return Image{};
        }
        
        
        Image image(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return image(arg_string);
        }
        
        Image image(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return image(arg_string);
        }
        
        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


        SizeDesc    size_desc(std::string_view k)
        {
            return SizeDesc(k);
        }
        
        SizeDesc    size_desc(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return size_desc(arg_string);
        }
        
        SizeDesc    size_desc(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return size_desc(arg_string);
        }

    }


////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////

    namespace html {
        WebHtml&    operator<<(WebHtml& h, Image i)
        {
            std::string n   = cdb::label(cdb::document(i));
            h << "<img src=\"/image?id=" << i.id << "\" alt=\"";
            html_escape_write(h, n);
            h << "\">";
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, Dev<Image>v)
        {
            h << "<a href=\"/dev/image?id=" << v.data.id << "\">" << cdb::thumbnail(v.data) << "</a>";
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, DevID<Image> v)
        {
            h << "<a href=\"/dev/image?id=" << v.data.id << "\">" << v.data.id << "</a>";
            return h;
        }

        WebHtml&    operator<<(WebHtml&h, SizeDesc sz)
        {
            switch(sz){
            case SizeDesc::Original:
                h << "original";
                break;
            case SizeDesc::Large:
                h << "large";
                break;
            case SizeDesc::Medium:
                h << "medium";
                break;
            case SizeDesc::Small:
            default:
                h << "small";
                break;
            }
            return h;
        }

        
        WebHtml&    operator<<(WebHtml& h, const Thumbnail& t)
        {
            std::string n   = t.alt;
            if(n.empty())
                n           = "Thumbnail for '" + cdb::label(cdb::document(t.img)) + "'";
            cdb::label(cdb::document(t.img));
            h << "<img src=\"/thumbnail?id=" << t.img.id;
            //if(t.size != SizeDesc())
                //h << "&size=" << t.size.key();
            h << "\" alt=\"";
            html_escape_write(h, n);
            h << "'\" class=\"" << t.size << "\"/>";
            return h;
        }

        void        dev_table(WebHtml& h, const std::vector<Image>& images)
        {
            auto t = h.table();
            h << "<tr><th>ID</th><th>Thumb</th><th>Fragment</th></tr>\n";
            for(Image i : images)
                h << "<tr><td>" << dev_id(i) << "</td><td>" << cdb::thumbnail(i, SizeDesc::Small) << "</td><td>" 
                    << dev(cdb::fragment(i)) << "</td></tr>\n";
        }
    }

////////////////////////////////////////////////////////////////////////////////

    namespace post {

        Image image(WebContext&ctx, bool *detected)
        {
            ctx.decode_post();
            
            if(detected)
                *detected   = false;
                
            std::string    k    = ctx.rx_post.first("image");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return arg::image(k);
            }
            return Image();
        }
        
        Image image(WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_name));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::image(arg_string);
        }
        
        Image image(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_names));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::image(arg_string);
        }
    }

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}

