////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ImageCDB.hpp"
#include <basic/ByteArray.hpp>
#include <kernel/db/SQ.hpp>
#include <kernel/file/DocumentCDB.hpp>
#include <kernel/file/FolderCDB.hpp>
#include <kernel/file/FragmentCDB.hpp>
#include <kernel/file/Root.hpp>

namespace yq {

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
                if(s.step() == SQResult::Row)
                    return s.v_bytes(1);
                return ByteArray();
            }

            ByteArray           bytes_medium(Image img)
            {
                static thread_local SQ s("SELECT medium FROM Images WHERE id=?");
                auto s_af   = s.af();
                s.bind(1, img.id);
                if(s.step() == SQResult::Row)
                    return s.v_bytes(1);
                return ByteArray();
            }

            ByteArray          bytes_small(Image img)
            {
                static thread_local SQ s("SELECT small FROM Images WHERE id=?");
                auto s_af   = s.af();
                s.bind(1, img.id);
                if(s.step() == SQResult::Row)
                    return s.v_bytes(1);
                return ByteArray();
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
            return bytes(img, sz);
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
            if(s.step() == SQResult::Row)
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
    }
}

