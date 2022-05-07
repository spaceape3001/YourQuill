////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/db/Sq.hpp>

namespace yq {
    namespace cdb {
        Vector<Image>           all_images()
        {
            static thread_local SQ s("SELECT id FROM Images");
            return s.vec<Image>();
        }
        
        size_t                  all_images_count()
        {
            static thread_local SQ s("SELECT COUNT(1) FROM Images");
            return s.size();
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
            auto i_af   = i.af();
            i.bind(1, frag.id);
            i.bind(2, mimeTypeForExt(suffix(frag)));
            
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
}
