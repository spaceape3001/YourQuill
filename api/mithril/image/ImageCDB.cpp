////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "ImageCDB.hpp"
#include <yq/container/ByteArray.hpp>

#include <mithril/document/DocumentCDB.hpp>
#include <mithril/folder/FolderCDB.hpp>
#include <mithril/fragment/FragmentCDB.hpp>
#include <mithril/root/RootCDB.hpp>
#include <mithril/root/RootDir.hpp>
#include <mithril/wksp/CacheQuery.hpp>
#include <yq/text/format.hpp>

namespace yq::mithril::cdb {
    std::vector<Image>      all_images(Sorted sorted)
    {
        static thread_local CacheQuery s("SELECT id FROM " TBL_IMAGES "");
        return s.vec<Image>();
    }
    
    size_t                  all_images_count()
    {
        static thread_local CacheQuery s("SELECT COUNT(1) FROM " TBL_IMAGES "");
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
            static thread_local CacheQuery s("SELECT large FROM " TBL_IMAGES " WHERE id=?");
            return s.bytes(img);
        }

        ByteArray           bytes_medium(Image img)
        {
            static thread_local CacheQuery s("SELECT medium FROM " TBL_IMAGES " WHERE id=?");
            return s.bytes(img);
        }

        ByteArray          bytes_small(Image img)
        {
            static thread_local CacheQuery s("SELECT small FROM " TBL_IMAGES " WHERE id=?");
            return s.bytes(img);
        }
    }
    
    ByteArray           bytes(Image img, SizeDesc sz)
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

    Image               db_image(Fragment frag, bool *wasCreated)
    {
        static thread_local CacheQuery i("INSERT OR FAIL INTO " TBL_IMAGES " (id,type) VALUES (?,?)");
        
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

    bool                exists(Image i)
    {
        return exists_image(i.id);
    }

    bool                exists_image(uint64_t i)
    {
        static thread_local CacheQuery s("SELECT 1 FROM " TBL_IMAGES " WHERE id=? LIMIT 1");
        return s.present(i);
    }

    Fragment            fragment(Image i)
    {
        return exists(i) ? Fragment{i.id} : Fragment{};
    }
    
    Image               icon(const RootDir* rt)
    {
        if(!rt)
            return Image();
        static thread_local CacheQuery s("SELECT icon FROM " TBL_ROOTS " WHERE root=?");
        return s.as<Image>(rt->id);
    }

    Image               icon(Image img)
    {
        return img;
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
        return bytes(img, sz);
    }

    Image::Info         info(Image img)
    {
        static thread_local CacheQuery s("SELECT type, width, height FROM " TBL_IMAGES " WHERE id=?");
        auto s_af = s.af();
        s.bind(1, img.id);
        Image::Info     ret{};
        if(s.step() == SQResult::Row){
            ret.type    = ContentType( s.v_int(1) );
            ret.dim.x   = s.v_int(2);
            ret.dim.y   = s.v_int(3);
        }
        return ret;
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
    
    std::string             key(Image i)
    {
        return key(document(i));
    }

    ContentType         mime_type(Image i)
    {
        static thread_local CacheQuery s("SELECT type FROM " TBL_IMAGES " WHERE id=?");
        auto s_af   = s.af();
        s.bind(1, i.id);
        if(s.step() == SQResult::Row)
            return ContentType( s.v_int(1));
        return ContentType();
    }

    uint64_t                modified(Image img)
    {
        return modified(fragment(img));
    }

    std::string             name(Image i)
    {
        return to_string(i.id);
    }

    Thumbnail               thumbnail(Image i, SizeDesc sz) 
    { 
        return { i, sz }; 
    }
    
    Thumbnail               thumbnail(Image i, std::string_view alt, SizeDesc sz)
    {
        return { i, sz, std::string(alt) }; 
    }
}
