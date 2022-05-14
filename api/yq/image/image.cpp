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

#include <yq/db/SQ.hpp>
#include <yq/file/DocumentCDB.hpp>
#include <yq/file/FolderCDB.hpp>
#include <yq/file/FragmentCDB.hpp>
#include <yq/file/FragmentHtml.hpp>
#include <yq/file/Root.hpp>
#include <yq/stream/Ops.hpp>
#include <yq/text/text_utils.hpp>
#include <yq/type/ByteArray.hpp>
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

    namespace cdb {
        std::vector<Image>      all_images()
        {
            static thread_local SQ s("SELECT id FROM Images");
            return s.vec<Image>();
        }
        
        size_t                  all_images_count()
        {
            static thread_local SQ s("SELECT COUNT(1) FROM Images");
            return s.size();
        }

        Image   best_image(Document x)
        {
            std::string k   = skeyc(x);
            if(k.empty())
                return Image{};
                
            k += '.';
                
            //size_t  i   = k.find_last_of('.');
            //if((i != std::string::npos) && (i>0)){
                //// truncate
                //k.resize(i+1);
            //}
            
            Folder      fo  = folder(x);
            for(const char* z : Image::kSupportedExtensions){
                std::string     k2 = k + z;
                Document        dimg    = child_document(fo, k2);
                if(!dimg)
                    continue;
                Image img = image(dimg);
                if(img)
                    return img;
            }
            return Image{};
        }

        namespace {
        
            ByteArray          bytes_image(Image img)
            {
                return frag_bytes(Fragment{img.id}, DONT_LOCK);
            }
            
            ByteArray           bytes_large(Image img)
            {
                static thread_local SQ s("SELECT large FROM Images WHERE id=?");
                auto s_af   = s.af();
                s.bind(1, img.id);
                if(s.step() == SqlQuery::Row)
                    return s.v_bytes(1);
                return ByteArray();
            }

            ByteArray           bytes_medium(Image img)
            {
                static thread_local SQ s("SELECT medium FROM Images WHERE id=?");
                auto s_af   = s.af();
                s.bind(1, img.id);
                if(s.step() == SqlQuery::Row)
                    return s.v_bytes(1);
                return ByteArray();
            }

            ByteArray          bytes_small(Image img)
            {
                static thread_local SQ s("SELECT small FROM Images WHERE id=?");
                auto s_af   = s.af();
                s.bind(1, img.id);
                if(s.step() == SqlQuery::Row)
                    return s.v_bytes(1);
                return ByteArray();
            }
        }
        
        ByteArray           bytes(Image img, SizeDesc sz)
        {
            switch(sz){
            case SizeDesc::Large:
                return bytes_large(img);
            case SizeDesc::Medium:
                return bytes_medium(img);
            case SizeDesc::Small:
                return bytes_small(img);
            case SizeDesc::Original:
            default:
                return bytes_image(img);
            }
        }

        Image               db_image(Fragment frag, bool *wasCreated)
        {
            static thread_local SQ i("INSERT OR FAIL INTO Images (id,type) VALUES (?,?)");
            
            auto ext    = suffix(frag).ext;
            auto i_af   = i.af();
            i.bind(1, frag.id);
            i.bind(2, mimeTypeForExt(ext));
            
            if(is_good(i.step())){
                if(wasCreated)
                    *wasCreated = true;
                return Image{frag.id};
            } else if(exists_image(frag.id)){
                return Image{frag.id};
            } else 
                return Image{};
        }

        Document            document(Image i)
        {
            return document(fragment(i));
        }

        void                erase(Image x)
        {
            static thread_local SQ stmts[] = {
                SQ("UPDATE Atoms SET icon=0 WHERE icon=?"),
                SQ("UPDATE Categories SET icon=0 WHERE icon=?"),
                SQ("UPDATE Classes SET icon=0 WHERE icon=?"),
                SQ("UPDATE Documents SET icon=0 WHERE icon=?"),
                SQ("UPDATE Fields SET icon=0 WHERE icon=?"),
                SQ("UPDATE Leafs SET icon=0 WHERE icon=?"),
                SQ("UPDATE Tags SET icon=0 WHERE icon=?"),
                SQ("DELETE FROM Images WHERE id=?")
            };
            
            if(!x)
                return ;
                
            for(auto& sq : stmts)
                sq.exec(x.id);
        }

        bool                exists(Image i)
        {
            return exists_image(i.id);
        }

        bool                exists_image(uint64_t i)
        {
            static thread_local SQ s("SELECT 1 FROM Images WHERE id=? LIMIT 1");
            return s.present(i);
        }

        Fragment            fragment(Image i)
        {
            return exists(i) ? Fragment{i.id} : Fragment{};
        }
        
        Image               icon(const Root* rt)
        {
            if(!rt)
                return Image();
            static thread_local SQ s("SELECT icon FROM RootIcons WHERE root=?");
            return s.as<Image>(rt->id);
        }

        Image               image(std::string_view k)
        {
            return image(document(k));
        }

        Image               image(Document d)
        {
            return image(first_fragment(d, DataRole::Image));
        }

        Image               image(Fragment f)
        {
            return exists_image(f.id) ? Image{f.id} : Image{};
        }
        
        Image               image(uint64_t i)
        {
            return exists_image(i) ? Image{i} : Image{};
        }

        ByteArray          image_bytes(Image img, SizeDesc sz)
        {
            if(is_raster(img)){
                switch(sz){
                case SizeDesc::Large:
                    return bytes_large(img);
                case SizeDesc::Medium:
                    return bytes_medium(img);
                case SizeDesc::Small:
                    return bytes_small(img);
                case SizeDesc::Original:
                default:
                    return bytes_image(img);
                }
            } else
                return bytes_image(img);
        }

        bool                is_raster(ContentType ct)
        {
            switch(ct){
            case ContentType::gif:
            case ContentType::jpeg:
            case ContentType::png:
            case ContentType::tiff:
                return true;
            default:
                return false;
            }
        }
        
        bool                is_raster(Image i)
        {
            return is_raster(mime_type(i));
        }
        
        ContentType         mime_type(Image i)
        {
            static thread_local SQ s("SELECT type FROM Images WHERE id=?");
            auto s_af   = s.af();
            s.bind(1, i.id);
            if(s.step() == SqlQuery::Row)
                return ContentType( s.v_int(1));
            return ContentType();
        }

        Thumbnail               thumbnail(Image i, SizeDesc sz) 
        { 
            return { i, sz }; 
        }
        
        Thumbnail               thumbnail(Image i, std::string_view alt, SizeDesc sz)
        {
            return { i, sz, std::string(alt) }; 
        }


        void                    update_root(const Root*rt, Image img)
        {
            if(!rt)
                return ;
                
            static thread_local SQ r("REPLACE INTO RootIcons (root, icon) VALUES (?, ?)");
            auto r_af = r.af();
            r.bind(1, rt->id);
            r.bind(2, img.id);
            r.step();
        }
    }


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
        
        WebHtml&    operator<<(WebHtml& h, const Thumbnail& t)
        {
            std::string n   = t.alt;
            if(n.empty())
                n           = "Thumbnail for '" + cdb::label(cdb::document(t.img)) + "'";
            cdb::label(cdb::document(t.img));
            h << "<img src=\"/thumbnail?id=" << t.img.id;
            if(t.size != SizeDesc())
                h << "&size=" << t.size.key();
            h << "\" alt=\"";
            html_escape_write(h, n);
            h << "'\" />";
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
////////////////////////////////////////////////////////////////////////////////
}

