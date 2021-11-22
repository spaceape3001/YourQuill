////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

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
    
        QByteArray          bytes_image(Image img)
        {
            static thread_local SQ s("SELECT image FROM Images WHERE id=?");
            auto s_af   = s.af();
            s.bind(0, img.id);
            if(s.exec() && s.next())
                return s.valueBytes(0);
            return QByteArray();
        }
        
        QByteArray          bytes_large(Image img)
        {
            static thread_local SQ s("SELECT large FROM Images WHERE id=?");
            auto s_af   = s.af();
            s.bind(0, img.id);
            if(s.exec() && s.next())
                return s.valueBytes(0);
            return QByteArray();
        }

        QByteArray          bytes_medium(Image img)
        {
            static thread_local SQ s("SELECT medium FROM Images WHERE id=?");
            auto s_af   = s.af();
            s.bind(0, img.id);
            if(s.exec() && s.next())
                return s.valueBytes(0);
            return QByteArray();
        }

        QByteArray          bytes_small(Image img)
        {
            static thread_local SQ s("SELECT small FROM Images WHERE id=?");
            auto s_af   = s.af();
            s.bind(0, img.id);
            if(s.exec() && s.next())
                return s.valueBytes(0);
            return QByteArray();
        }
    }
    
    QByteArray          bytes(Image img, SizeDesc sz)
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
        i.bind(0, frag.id);
        i.bind(1, mimeTypeForExt(suffix(frag)));
        bool    f   = i.exec(false);
        if(wasCreated)
            *wasCreated = f;
        return Image{frag.id};
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

    Image               image(const String&k)
    {
        return image(document(k));
    }

    Image               image(Document d)
    {
        return image(first(d, DataRole::Image));
    }

    Image               image(Fragment f)
    {
        return exists_image(f.id) ? Image{f.id} : Image{};
    }
    
    Image               image(uint64_t i)
    {
        return exists_image(i) ? Image{i} : Image{};
    }

    QByteArray          image_bytes(Image img, SizeDesc sz)
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

    QIcon               qIcon(Image img)
    {
        if(!img)
            return QIcon();
        return QIcon(path(Fragment{img.id}).c_str());
    }

    bool                raster(ContentType ct)
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
    
    bool                raster(Image i)
    {
        return raster(type(i));
    }
    

    ContentType         type(Image i)
    {
        static thread_local SQ s("SELECT type FROM Images WHERE id=?");
        return ContentType(s.str(i.id));
    }

    void                    update_root(const Root*rt, Image img)
    {
        if(!rt)
            return ;
            
        static thread_local SQ r("REPLACE INTO RootIcons (root, icon) VALUES (?, ?)");
        auto r_af = r.af();
        r.bind(0, rt->id);
        r.bind(1, img.id);
        r.exec();
    }
}
