////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "FileSys.hpp"
#include "Image.hpp"
#include "yq/Loggers.hpp"
#include "yq/sql/SqlQuery.hpp"
#include "yq/util/Utilities.hpp"
#include "yq/wksp/Workspace.hpp"

#include <QBuffer>
#include <QImage>
#include <QImageReader>
#include <QSqlError>

namespace cdb {
    Vector<Image>           all_images()
    {
        Vector<Image>   ret;
        static thread_local SqlQuery s(wksp::cache(), "SELECT id FROM Images");
        auto s_af = s.af();
        if(s.exec()){
            while(s.next())
                ret << Image{ s.valueU64(0) };
        }
        return ret;
    }
    
    size_t                  all_images_count()
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT COUNT(1) FROM Images");
        auto s_af   = s.af();
        if(s.exec() && s.next())
            return (size_t) s.valueU64(0);
        return 0;
    }

    namespace {
    
        QByteArray          bytes_image(Image img)
        {
            static thread_local SqlQuery s(wksp::cache(), "SELECT image FROM Images WHERE id=?");
            auto s_af   = s.af();
            s.bind(0, img.id);
            if(s.exec() && s.next())
                return s.valueBytes(0);
            return QByteArray();
        }
        
        QByteArray          bytes_large(Image img)
        {
            static thread_local SqlQuery s(wksp::cache(), "SELECT large FROM Images WHERE id=?");
            auto s_af   = s.af();
            s.bind(0, img.id);
            if(s.exec() && s.next())
                return s.valueBytes(0);
            return QByteArray();
        }

        QByteArray          bytes_medium(Image img)
        {
            static thread_local SqlQuery s(wksp::cache(), "SELECT medium FROM Images WHERE id=?");
            auto s_af   = s.af();
            s.bind(0, img.id);
            if(s.exec() && s.next())
                return s.valueBytes(0);
            return QByteArray();
        }

        QByteArray          bytes_small(Image img)
        {
            static thread_local SqlQuery s(wksp::cache(), "SELECT small FROM Images WHERE id=?");
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

    Image               db_image(Document doc, bool *wasCreated)
    {
        static thread_local SqlQuery i(wksp::cache(), "INSERT OR FAIL INTO Images (id,type) VALUES (?,?)");
        auto i_af   = i.af();
        i.bind(0, doc.id);
        i.bind(1, mimeTypeForExt(suffix(doc)));
        bool    f   = i.exec(false);
        if(wasCreated)
            *wasCreated = f;
        return exists_image(doc.id) ?  Image{doc.id} : Image{};
    }

    Document            document(Image i)
    {
        return exists(i) ? Document{i.id} : Document{};
    }

    bool                exists(Image i)
    {
        return exists_image(i.id);
    }

    bool                exists_image(uint64_t i)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT 1 FROM Images WHERE id=? LIMIT 1");
        auto s_lk   = s.af();
        s.bind(0, i);
        if(s.exec() && s.next())
            return s.valueAs<bool>(0);
        return false;
    }

    Image               image(Document d)
    {
        return image(d.id);
    }
    
    Image               image(uint64_t i)
    {
        return exists_image(i) ? Image{i} : Image{};
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
        static thread_local SqlQuery s(wksp::cache(), "SELECT type FROM Images WHERE id=?");
        auto s_af   = s.af();
        s.bind(0, i.id);
        if(s.exec() && s.next())
            return ContentType(s.valueString(0));
        return ContentType();
    }

    void                update(Image i)
    {
        if(!i.id)
            return ;
            
        Document    doc(i.id);
        Fragment    frag    = first(doc, DataRole::Image);
        QByteArray  byt     = bytes(frag);
        
        static thread_local SqlQuery u1(wksp::cache(), "UPDATE Images SET image=? WHERE id=?");
        auto u1_af  = u1.af();
        u1.bind(0, byt);
        u1.bind(1, i.id);
        u1.exec();
        
        ContentType ct  = type(i);
        if(ct == ContentType::svg)
            return ;

        QImageReader    reader(path(frag));
        QByteArray      fmt     = reader.format();
        
        auto ct2        = mimeTypeForExt(utf8(fmt));
        if(ct2 != ct){
                // Cover the case of a mis-labeled type
            static thread_local SqlQuery u2(wksp::cache(), "UPDATE Images SET type=? WHERE id=?");
            auto u2_af = u2.af();
            u2.bind(0, ct2.key());
            u2.bind(1, i.id);
            u2.exec();
        }
        
        QImage  img     = reader.read();
        QImage  sm      = img.scaled(Image::kSmallWidth,  Image::kSmallHeight,  Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QImage  med     = img.scaled(Image::kMediumWidth, Image::kMediumHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QImage  large   = img.scaled(Image::kLargeWidth,  Image::kLargeHeight,  Qt::KeepAspectRatio, Qt::SmoothTransformation);
        
        QBuffer bs, bm, bl;
        bs.open(QIODevice::WriteOnly);
        sm.save(&bs, fmt.data());
        bs.close();
        bm.open(QIODevice::WriteOnly);
        med.save(&bm, fmt.data());
        bm.close();
        bl.open(QIODevice::WriteOnly);
        large.save(&bl, fmt.data());
        bl.close();
        
        static thread_local SqlQuery u3(wksp::cache(), "UPDATE Images SET small=?,medium=?,large=? WHERE id=?");
        auto u3_af = u3.af();
        u3.bind(0, bs.data());
        u3.bind(1, bm.data());
        u3.bind(2, bl.data());
        u3.bind(3, i.id);
        u3.exec();
    }
}
